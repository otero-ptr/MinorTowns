#include "Game.h"
#include "gen_uuid.h"
#include "User.h"
#include "Army/Battle/Battle.h"
#include "GameController/GameNotify.h"
#include "GameSettings.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

Game::Game(std::vector<std::shared_ptr<User>> users, std::shared_ptr<GameSettings> settings) :
	game_controller(settings->getMaxTick(), settings->getRepeatTick()),
	uuid(GenerateUUID()),
	cooldown_tick(settings->getCooldownTick()),
	logger(std::make_unique<spdlog::logger>(uuid))
{
	auto log_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Logs/Game/game[" + uuid + "].log", true);
	log_file_sink->set_pattern("%^[%l] [%d-%m-%Y %H:%M:%S.%e] [%t] %v%$");
	logger->sinks().push_back(log_file_sink);
	logger->set_level(spdlog::level::trace);
	logger->flush_on(spdlog::level::trace);

	logger->info("Game created");
	logger->info("game uuid: " + uuid);
	logger->info("Game settings:");
	logger->info("max tick: " + std::to_string(settings->getMaxTick()));
	logger->info("repeat tick: " + std::to_string(settings->getRepeatTick()));
	logger->info("cooldown tick: " + std::to_string(settings->getCooldownTick()));

	logger->info("users count: " + std::to_string(users.size()));
	this->users.reserve(users.size());
	for (auto& user : users) {
		logger->info("username: " + user->getUsername() + " [" + user->getUUID() + "]");
		this->users.push_back(user);
	}
}

Game::~Game()
{
	if (this->th_tick.joinable()) {
		this->th_tick.join();
	}
	logger->info("Game end");
}

void Game::start()
{
	std::lock_guard<std::mutex> lock(mtx);
	logger->info("Game start");
	this->game_controller.notify(users, GameNotify::GAME_START);
	this->th_tick = std::jthread(&Game::tick, this, th_tick.get_stop_token());
}

void Game::prepare(std::shared_ptr<GameSettings> settings)
{
	std::lock_guard<std::mutex> lock(mtx);
	logger->info("Game prepare");
	game_controller.notify(users, GameNotify::GAME_STARTING);
	logger->info("Create map");
	std::vector<int> idTowns = createMap(users.size());
	logger->info("Create towns");
	createTowns(idTowns, settings);

	logger->info("Notify map");
	notifyMap();
	logger->info("Notify table");
	notifyTable();
}

const std::string Game::getUUID()
{
	return uuid;
}

bool Game::isActive()
{
	/*
	If a stop request has been made, 
	the method returns true, otherwise - false
	true - game active
	false - game inactive
	*/
	return !th_tick.get_stop_token().stop_requested(); 
}

void Game::buildBuildings(std::shared_ptr<User>& user, int& building_type)
{
	for (auto& town : towns) {
		if (town.getOwnTown() == user) {
			logger->info(user->getUsername() + "[" + user->getUUID() + "] built a building " + std::to_string(building_type));
			town.buildBuilding(static_cast<TypeBuildings>(building_type));
			break;
		}
	}
}

void Game::raiseArmy(std::shared_ptr<User>& user, int& count_soldiers)
{
	std::lock_guard<std::mutex> lock(mtx);
	if (count_soldiers > 0) {
		for (int i = 0; i < towns.size(); ++i) {
			if (towns[i].getOwnTown() == user) {
				armies[i].merge(count_soldiers);
				break;
			}
		}
	}
	else {
		throw std::invalid_argument("The value cannot be less than 1.");
	}
}

void Game::disbandArmy(std::shared_ptr<User>& user, int& count_soldiers)
{
	std::lock_guard<std::mutex> lock(mtx);
	if (count_soldiers > 0) {
		for (int i = 0; i < towns.size(); ++i) {
			if (towns[i].getOwnTown() == user) {
				armies[i].merge(count_soldiers);
				break;
			}
		}
	}
	else {
		throw std::invalid_argument("The value cannot be less than 1.");
	}
}

void Game::attackArmy(std::shared_ptr<User>& user)
{
	for (int i = 0; i < towns.size(); ++i) {
		if (towns[i].getOwnTown() == user) {
			for (int a = 0; a < armies.size(); ++a) {
				if (armies[i].getNode() == armies[a].getNode() && i != a 
					&& armies[i].getCount() > 0 && armies[a].getCount() > 0) {
					battles.emplace_back(armies[a], armies[i], armies[i].getNode());
				}
			}
			break;
		}
	}
}

void Game::defeated(std::shared_ptr<User>& user) {
	std::lock_guard<std::mutex> lock(mtx);
	logger->info("user:" + user->getUsername() + "defeated");
	auto it_user = std::find(users.begin(), users.end(), user);
	if (it_user != users.end()) {
		logger->info("user:" + user->getUsername() + " remove user");
		users.erase(it_user);
		users.shrink_to_fit();
	}
	auto it_town = std::find_if(towns.begin(), towns.end(), [&user](const Town& town) {return town.getOwnTown() == user; });
	if (it_town != towns.end()) {
		logger->info("user:" + user->getUsername() + " remove town");
		towns.erase(it_town);
		towns.shrink_to_fit();
	}
}

void Game::tick(std::stop_token token)
{
	// start zero tick
	logger->trace("zero tick");
	mtx.lock();
	notifyTick(); 
	game_controller.control(tick_count, towns);
	mtx.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(cooldown_tick));
	// end zero tick

	while (!token.stop_requested()) {
		auto start = std::chrono::steady_clock::now();
		mtx.lock();
		++tick_count;
		for (auto& town : towns) {
			town.TownTickProcessing();
		}

		std::sort(towns.rbegin(), towns.rend());

		logger->trace("tick " + std::to_string(tick_count));
		notifyTick();

		game_controller.control(tick_count, towns);

		if (game_controller.isGameEnd()) {
			for (auto& town : towns) {
				town.getOwnTown()->setLocation(Location::MENU, "menu");
			}
			th_tick.request_stop();
			mtx.unlock();
			break;
		}
		mtx.unlock();

		auto end = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		std::this_thread::sleep_for(std::chrono::milliseconds(cooldown_tick - duration.count()));
	}
}

void Game::createTowns(std::vector<int> &id_towns, std::shared_ptr<GameSettings> settings)
{
	towns.reserve(users.size());
	armies.reserve(users.size());
	
	for (int i = 0; i < users.size(); i++) {
		towns.emplace_back(i, users[i], id_towns[i], settings->makeEconomy(),
			settings->makeChurch(), settings->makeManufactory());
		armies.emplace_back(0, id_towns[i]);
	}
}

void Game::notifyTick()
{
	nlohmann::json common_obj = makeCommonJson();
	for (auto& town : towns) {
		nlohmann::json info = createTownJson(town.getData(), common_obj);
		town.getOwnTown()->message_pool.push(info.dump());
	}
}

void Game::notifyMap()
{
	for (auto& town : towns) {
		town.getOwnTown()->message_pool.push(game_map->getMapJson());
	}
}

void Game::notifyTable()
{
	nlohmann::json common_obj;
	common_obj["table"] = makeTableJson();
	logger->info("game table: " + common_obj.dump());
	for (auto& town : towns) {
		town.getOwnTown()->message_pool.push(common_obj.dump());
	}
}

nlohmann::json Game::makeCommonJson()
{
	nlohmann::json common_obj;
	common_obj["tick"] = tick_count;
	common_obj["uuid"] = uuid;
	for (size_t it = 0; it < towns.size(); ++it) {
		auto result = towns[it].getData();
		common_obj["towns"][it]["town_id"] = result.town_id;
		common_obj["towns"][it]["username"] = result.username;
		common_obj["towns"][it]["net_worth"] = result.economy.net_worth;
		common_obj["towns"][it]["soldiers"] = result.army.count_soldiers;
		common_obj["towns"][it]["locate_army"] = result.army.locate_node;
	}
	return common_obj;
}

nlohmann::json Game::makeTableJson()
{
	nlohmann::json common_obj;
	for (size_t it = 0; it < towns.size(); ++it) {
		auto result = towns[it].getData();
		common_obj[it]["town_id"] = result.town_id;
		common_obj[it]["username"] = result.username;
	}
	return common_obj;
}

nlohmann::json Game::createTownJson(const TownData&& data, const nlohmann::json& common_json_obj) {
	nlohmann::json unique_obj;
	unique_obj = common_json_obj;
	unique_obj["town"]["town_id"] = data.town_id;
	unique_obj["town"]["economy"]["budget"] = data.economy.budget;
	unique_obj["town"]["economy"]["multiplier"] = std::round(static_cast<double>(data.economy.multiplier) * 100) / 100;
	unique_obj["town"]["economy"]["income"] = data.economy.income;
	unique_obj["town"]["economy"]["detail"]["charch_bonus"] = std::round(static_cast<double>(data.charch.bonus) * 100) / 100;
	unique_obj["town"]["economy"]["detail"]["manufactury_bonus"] = data.manufactory.bonus;
	unique_obj["town"]["economy"]["detail"]["army_cost"] = data.army.cost;
	unique_obj["town"]["buildings"]["charch_count"] = data.charch.count;
	unique_obj["town"]["buildings"]["charch_price"] = data.charch.price;
	unique_obj["town"]["buildings"]["manufactory_count"] = data.manufactory.count;
	unique_obj["town"]["buildings"]["manufactory_price"] = data.manufactory.price;
	return unique_obj;
}

std::vector<int> Game::createMap(size_t count_user)
{
	auto size_map = DimensionMap::detect(count_user);
	logger->info("Map size: " + std::to_string(size_map.x) + 
		"x" + std::to_string(size_map.y));
	auto pos_towns = DimensionMap::placeTowns(count_user, size_map);
	for (auto &[x,y] : pos_towns) {
		logger->info("pos town: " + std::to_string(x) + "x" + std::to_string(y));
	}
	game_map = std::make_unique<GameMap>(size_map);
	auto result = game_map->placeTowns(pos_towns);
	logger->info("map: " + game_map->getMapJson());
	return result;
}

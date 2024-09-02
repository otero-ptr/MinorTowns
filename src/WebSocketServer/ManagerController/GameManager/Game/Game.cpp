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
	for (auto& user : users) {
		logger->info("username: " + user->getUsername() + " [" + user->getUUID() + "]");
	}

	prepare(users, settings);
}

Game::~Game()
{
	if (th_tick.joinable()) {
		th_tick.join();
	}
	logger->info("Game end");
}

void Game::start()
{
	std::lock_guard<std::mutex> lock(mtx);
	logger->info("Game start");
	game_controller.notify(leaderboard, GameNotify::GAME_START);
	th_tick = std::jthread(&Game::tick, this, th_tick.get_stop_token());
}

void Game::prepare(std::vector<std::shared_ptr<User>>& users, std::shared_ptr<GameSettings> settings)
{
	std::lock_guard<std::mutex> lock(mtx);
	logger->info("Game prepare");
	game_controller.notify(users, GameNotify::GAME_STARTING);
	logger->info("Create map");
	std::vector<uint8_t> id_towns = createMap(users.size());
	logger->info("Create Game Data");
	createGameData(users, id_towns, settings);

	logger->info("Notify map");
	notifyMap();
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

void Game::buildBuildings(std::shared_ptr<User>& user, int building_type)
{
	towns[user->getUUID()].second->buildBuilding(static_cast<TypeBuildings>(building_type));
	logger->info(user->getUsername() + "[" + user->getUUID() + "] built a building " + std::to_string(building_type));
}

void Game::raiseArmy(std::shared_ptr<User>& user, uint32_t soldiers)
{
	std::lock_guard<std::mutex> lock(mtx);
	auto& [army_data, army] = armies[user->getUUID()];
	auto& [town_data, town] = towns[user->getUUID()];
	if (army_data.node_id == town_data.node_id) {
		if (town->allocateForArmy(soldiers)) {
			armies[user->getUUID()].second->merge(soldiers);
		}
	}
}

void Game::disbandArmy(std::shared_ptr<User>& user, uint32_t soldiers)
{
	std::lock_guard<std::mutex> lock(mtx);
	auto& [army_data, army] = armies[user->getUUID()];
	auto& [town_data, town] = towns[user->getUUID()];
	if (army_data.node_id == town_data.node_id) {
		if (soldiers > army->getCount()) {
			soldiers = army->getCount();
		}
		if (town->deallocateForArmy(soldiers)) {
			armies[user->getUUID()].second->detach(soldiers);
		}
	}
}

void Game::moveArmy(std::shared_ptr<User>& user, uint8_t node)
{
	auto& [data, army] = armies[user->getUUID()];
	auto way = game_map->buildWay(data.node_id, node);
	data.way = way;
}

void Game::tick(std::stop_token token)
{
	while (!token.stop_requested()) {
		auto start = std::chrono::steady_clock::now();
		mtx.lock();

		logger->trace("tick " + std::to_string(game_controller.getTick()));

		checkBankruptcy();

		leaderboardProcessing();

		armyProcessing();

		notifyTick();

		game_controller.control(leaderboard);

		if (game_controller.isEnd()) {
			for (auto& [user, town] : leaderboard) {
				user->setLocation(Location::MENU, "menu");
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

void Game::createGameData(std::vector<std::shared_ptr<User>>& users, const std::vector<uint8_t> &id_towns, std::shared_ptr<GameSettings> settings)
{
	leaderboard.reserve(users.size());
	for (uint8_t i = 0; i < users.size(); ++i) {
		TownData town_data{};
		town_data.node_id = id_towns[i];
		auto town = std::make_shared<Town>(
			i, settings->makeEconomy(),
			settings->makeChurch(), settings->makeManufactory(), 
			settings->getTax(),
			settings->getPriceConvocation(), settings->getPriceMaintenance());

		ArmyData army_data;
		army_data.node_id = id_towns[i];
		auto army = std::make_shared<Army>(0);

		leaderboard.emplace_back(users[i], town);
		armies.emplace(users[i]->getUUID(), std::make_pair(army_data, army));
		towns.emplace(users[i]->getUUID(), std::make_pair(town_data, town));
	}
}

void Game::notifyTick()
{
	nlohmann::json common_obj = makeCommonJson();
	for (auto& [user, town] : leaderboard) {
		nlohmann::json info = createTownJson(town->getDetails(), common_obj);
		user->message_pool.push(info.dump());
	}
}

void Game::notifyMap()
{
	for (auto& [user, town] : leaderboard) {
		user->message_pool.push(game_map->getMapJson());
	}
}

nlohmann::json Game::makeCommonJson()
{
	nlohmann::json common_obj;
	common_obj["tick"] = game_controller.getTick();
	common_obj["uuid"] = uuid;
	size_t it = 0;
	for (auto& [user, town] : leaderboard) {
		auto result = town->getDetails();
		auto &location_town = towns[user->getUUID()].first;
		common_obj["towns"][it]["town_id"] = result.town_id;
		common_obj["towns"][it]["username"] = user->getUsername();
		common_obj["towns"][it]["net_worth"] = result.economy.net_worth;
		auto &[location_army, army] = armies[user->getUUID()];
		if (army->getCount() > 0) {
			common_obj["towns"][it]["soldiers"] = army->getCount();
			common_obj["towns"][it]["node_army"] = location_army.node_id;
		}
		common_obj["towns"][it]["node_town"] = location_town.node_id;
		++it;
	}
	return common_obj;
}

nlohmann::json Game::createTownJson(const TownDetails&& data, const nlohmann::json& common_json_obj) {
	nlohmann::json unique_obj;
	unique_obj = common_json_obj;
	unique_obj["town"]["town_id"] = data.town_id;
	unique_obj["town"]["economy"]["budget"] = data.economy.budget;
	unique_obj["town"]["economy"]["multiplier"] = std::round(static_cast<double>(data.economy.multiplier) * 100) / 100;
	unique_obj["town"]["economy"]["income"] = data.economy.income;
	unique_obj["town"]["economy"]["detail"]["charch_bonus"] = std::round(static_cast<double>(data.charch.bonus) * 100) / 100;
	unique_obj["town"]["economy"]["detail"]["manufactury_bonus"] = data.manufactory.bonus;
	unique_obj["town"]["economy"]["detail"]["army_cost"] = data.army.cost_tick;
	unique_obj["town"]["buildings"]["charch_count"] = data.charch.count;
	unique_obj["town"]["buildings"]["charch_price"] = data.charch.price;
	unique_obj["town"]["buildings"]["manufactory_count"] = data.manufactory.count;
	unique_obj["town"]["buildings"]["manufactory_price"] = data.manufactory.price;
	return unique_obj;
}

std::vector<uint8_t> Game::createMap(size_t count_user)
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

void Game::armyProcessing() {
	for (auto& [user_uuid, army] : armies) {
		if (!army.first.way.empty()) {
			army.first.node_id = army.first.way.front();
			army.first.way.pop_front();
		}
	}
}

void Game::leaderboardProcessing()
{
	for (auto& [user, town] : leaderboard) {
		town->processing();
	}
	sortLeaderboard();
}

void Game::sortLeaderboard()
{
	std::sort(leaderboard.rbegin(), leaderboard.rend(), [](const auto& a, const auto& b) {
		return *a.second < *b.second;
		});
}

void Game::checkBankruptcy()
{
	std::vector<std::shared_ptr<User>> to_remove;
	for (auto& [user, town] : leaderboard) {
		if (town->isBankrupt()) {
			to_remove.push_back(user);
		}
	}
	for (const auto& user : to_remove) {
		removingPlayer(user);
		user->message_pool.push("{\"defeated\": 2}");
	}
}


void Game::defeated(std::shared_ptr<User> user) {
	std::lock_guard<std::mutex> lock(mtx);
	removingPlayer(user);
	user->message_pool.push("{\"defeated\": 1}");
}

void Game::removingPlayer(std::shared_ptr<User> user)
{
	logger->info("user:" + user->getUsername() + " defeated");
	auto it_user = std::find_if(leaderboard.begin(), leaderboard.end(),
		[&user](const std::pair<std::shared_ptr<User>, std::shared_ptr<Town>>& place) {
			return place.first == user;
		}
	);

	if (it_user != leaderboard.end()) {
		logger->info("user:" + user->getUsername() + " remove user");
		leaderboard.erase(it_user);
	}
	if (towns.erase(user->getUUID()) > 0) {
		logger->info("user:" + user->getUsername() + " remove town");
	}
	if (armies.erase(user->getUUID()) > 0) {
		logger->info("user:" + user->getUsername() + " remove armies");
	}
}
#include "Game.h"
#include "util/gen_uuid.h"
#include "User/User.h"
#include "Army/Battle/Battle.h"
#include "GameController/GameNotify.h"


Game::Game(std::vector<std::weak_ptr<User>> users, std::shared_ptr<GameSettings> settings) :
	game_controller(settings->getMaxTick(), settings->getRepeatTick()),
	uuid(GenerateUUID()), action_manager(std::make_unique<ActionManager>()),
	cooldown_tick(settings->getCooldownTick())
{
	this->users.reserve(users.size());
	for (auto& user : users) {
		this->users.push_back(user);
	}
}

Game::~Game()
{
	if (this->th_tick.joinable()) {
		this->th_tick.join();
	}
}

void Game::start()
{
	this->game_controller.notify(users, GameNotify::GAME_START);
	this->th_tick = std::jthread(&Game::tick, this, th_tick.get_stop_token());
}

void Game::prepare(std::shared_ptr<GameSettings> settings)
{
	game_controller.notify(users, GameNotify::GAME_STARTING);
	std::vector<int> idTowns = createMap(users.size());
	createTowns(idTowns, settings);

	notifyMap();
	notifyTable();
}

const std::string Game::getUUID()
{
	return uuid;
}

bool Game::isActive()
{
	return th_tick.get_stop_token().stop_requested();
}

void Game::buildBuildings(std::shared_ptr<User>& user, int& building_type)
{
	for (auto& town : towns) {
		if (town.getOwnTown() == user) {
			town.buildBuilding(static_cast<TypeBuildings>(building_type));
			break;
		}
	}
}

void Game::raiseArmy(std::shared_ptr<User>& user, int& count_soldiers)
{
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

void Game::tick(std::stop_token token)
{
	// start zero tick
	notifyTick(); 
	game_controller.control(tick_count, towns);
	std::this_thread::sleep_for(std::chrono::milliseconds(cooldown_tick));
	// end zero tick

	while (!token.stop_requested()) {
		auto start = std::chrono::steady_clock::now();
		++tick_count;
		for (auto& town : towns) {
			town.TownTickProcessing();
		}

		std::sort(towns.rbegin(), towns.rend());

		notifyTick();

		game_controller.control(tick_count, towns);
		
		action_manager->doActions();

		if (game_controller.isGameEnd()) {
			for (auto& town : towns) {
				town.getOwnTown()->setLocation(Location::MENU, "menu");
			}
			th_tick.request_stop();
			break;
		}

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
		
		towns.emplace_back(i, users[i].lock(), id_towns[i], settings->makeEconomy(),
			settings->makeCharch(), settings->makeManufactory());
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

std::vector<int> Game::createMap(size_t countUser)
{
	auto sizeMap = DimensionMap::detect(countUser);
	auto posTowns = DimensionMap::placeTowns(countUser, sizeMap);
	game_map = std::make_unique<GameMap>(sizeMap);
	return game_map->placeTowns(posTowns);
}

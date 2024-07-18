#include "Game.h"
#include "util\gen_uuid.h"
#include "User/User.h"
#include "nlohmann/json.hpp"
#include "Army/Battle/Battle.h"

Game::Game(std::vector<std::shared_ptr<User>> users) :
	uuid(GenerateUUID()), action_manager(std::make_unique<ActionManager>()),
	game_settings(std::make_unique<GameSettings>())
{
	std::vector<int> idTowns = this->createMap(users.size());
	this->createTowns(idTowns, users);
	this->th_tick = std::jthread(&Game::tick, this);
}

Game::~Game()
{
	this->active = false;
	if (this->th_tick.joinable()) {
		this->th_tick.join();
	}
}

const std::string Game::getUUID()
{
	return this->uuid;
}

bool Game::isActive()
{
	return this->active;
}

void Game::buildBuildings(std::shared_ptr<User> user, int& building_type)
{
	for (auto& town : this->towns) {
		if (town.getOwnTown() == user) {
			town.buildBuilding(building_type);
			break;
		}
	}
}

void Game::raiseArmy(std::shared_ptr<User> user, int& count_soldiers)
{
	if (count_soldiers > 0) {
		for (int i = 0; i < this->towns.size(); ++i) {
			if (this->towns[i].getOwnTown() == user) {
				this->armies[i].merge(count_soldiers);
				break;
			}
		}
	}
	else {
		throw std::invalid_argument("The value cannot be less than 1.");
	}
}

void Game::disbandArmy(std::shared_ptr<User> user, int& count_soldiers)
{
	if (count_soldiers > 0) {
		for (int i = 0; i < this->towns.size(); ++i) {
			if (this->towns[i].getOwnTown() == user) {
				this->armies[i].merge(count_soldiers);
				break;
			}
		}
	}
	else {
		throw std::invalid_argument("The value cannot be less than 1.");
	}
}

void Game::attackArmy(std::shared_ptr<User> user)
{
	for (int i = 0; i < this->towns.size(); ++i) {
		if (this->towns[i].getOwnTown() == user) {
			for (int a = 0; a < this->armies.size(); ++a) {
				if (this->armies[i].getNode() == this->armies[a].getNode() && i != a 
					&& this->armies[i].getCount() > 0 && this->armies[a].getCount() > 0) {
					this->battles.emplace_back(this->armies[a], this->armies[i], this->armies[i].getNode());
				}
			}
			break;
		}
	}
}

std::string Game::getMapJSON()
{
	return this->game_map->getMapJson();
}

void Game::tick()
{
	// start zero tick
	this->notifyUsersTick(); 
	this->game_controller.control(this->tick_count, this->towns);
	std::this_thread::sleep_for(std::chrono::milliseconds(this->game_settings->cooldownTick));
	// end zero tick

	while (this->active) {
		auto start = std::chrono::steady_clock::now();
		++tick_count;
		for (auto& town : towns) {
			town.TownTickProcessing();
		}

		std::sort(this->towns.rbegin(), this->towns.rend());

		this->notifyUsersTick();

		this->game_controller.control(this->tick_count, this->towns);
		
		this->action_manager->doActions();

		if (this->game_controller.isGameEnd()) {
			for (auto& town : this->towns) {
				town.getOwnTown()->setLocation(Location::MENU, "menu");
			}
			this->active = false;
			break;
		}

		auto end = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		std::this_thread::sleep_for(std::chrono::milliseconds(this->game_settings->cooldownTick - duration.count()));
	}
}

void Game::createTowns(std::vector<int> &id_towns, std::vector<std::shared_ptr<User>> &users)
{
	this->towns.reserve(users.size());
	this->armies.reserve(users.size());
	
	Economy townEconomy(this->game_settings->economy.startBudget,
		this->game_settings->economy.startIncome,
		this->game_settings->economy.startMultiplier);
	Buildings townBuildings(this->game_settings->buildings.priceIncrease);
	townBuildings.setPriceBuildings(0, this->game_settings->buildings.priceChurch, this->game_settings->buildings.modifierChurch); //church
	townBuildings.setPriceBuildings(1, this->game_settings->buildings.priceManufactory, this->game_settings->buildings.modifierManufactory); //manufactory

	for (int i = 0; i < users.size(); i++) {
		this->towns.emplace_back(i, users[i], id_towns[i], townEconomy, townBuildings);
		this->armies.emplace_back(0, id_towns[i]);
	}
}

void Game::notifyUsersTick()
{
	nlohmann::json jsonObj;
	jsonObj["tick"] = this->tick_count;
	jsonObj["uuid"] = this->uuid;
	for (int it = 0; it < this->towns.size(); ++it) {
		jsonObj["towns"][it]["town_id"] = this->towns[it].getID();
		jsonObj["towns"][it]["username"] = this->towns[it].getOwnTown()->getUsername();
		jsonObj["towns"][it]["networth"] = this->towns[it].getTownEconomy().getNetWorth();
		jsonObj["towns"][it]["soldiers"] = this->armies[it].getCount();
		jsonObj["towns"][it]["soldiers"] = this->armies[it].getNode();
	}
	for (auto& town : this->towns) {
		nlohmann::json uniqueJsonObj;
		uniqueJsonObj = jsonObj;
		uniqueJsonObj["town"]["town_id"] = town.getID();
		uniqueJsonObj["town"]["budget"] = town.getTownEconomy().getBudget();
		uniqueJsonObj["town"]["multiplier"] = town.getTownEconomy().getMultiplier();
		uniqueJsonObj["town"]["income"] = town.getTownEconomy().getTickIncome();
		uniqueJsonObj["town"]["charch_id"] = 0;
		uniqueJsonObj["town"]["charch_count"] = town.getTownBuildings().getCountBuildings(0);
		uniqueJsonObj["town"]["charch_price"] = town.getTownBuildings().getPriceBuildings(0);
		uniqueJsonObj["town"]["manufactory_id"] = 1;
		uniqueJsonObj["town"]["manufactory_count"] = town.getTownBuildings().getCountBuildings(1);
		uniqueJsonObj["town"]["manufactory_price"] = town.getTownBuildings().getPriceBuildings(1);
		town.getOwnTown()->message_pool.pushBackMessage(uniqueJsonObj.dump());
	}
}

std::vector<int> Game::createMap(int countUser)
{
	auto sizeMap = DimensionMap::detect(countUser);
	auto posTowns = DimensionMap::placeTowns(countUser, sizeMap);
	this->game_map = std::make_unique<GameMap>(sizeMap);
	return this->game_map->placeTowns(posTowns);
}

#include "Game.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/UUID.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/Dynamic/Var.h"
#include "User/User.h"

Game::Game(std::vector<std::shared_ptr<User>> users)
{
	this->gameSettings = new GameSettings;
	this->createUUID();
	std::vector<int> idTowns = this->createMap(users.size());
	this->createTowns(idTowns, users);

	this->cooldownTick = this->gameSettings->cooldownTick;
	delete this->gameSettings;

	this->actionManager = std::make_unique<ActionManager>();
	
	this->active = true;
	this->thTick = std::jthread(&Game::tick, this);
}

Game::~Game()
{
	this->active = false;
	if (this->thTick.joinable()) {
		this->thTick.join();
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

void Game::buildBuildings(std::shared_ptr<User> user, int& buildingType)
{
	for (auto& town : this->towns) {
		if (town.getOwnTown() == user) {
			town.buildBuilding(buildingType);
			break;
		}
	}
}

void Game::raiseArmy(std::shared_ptr<User> user, int& countSoldiers)
{
	if (countSoldiers > 0) {
		for (int i = 0; i < this->towns.size(); ++i) {
			if (this->towns[i].getOwnTown() == user) {
				this->armies[i].merge(countSoldiers);
				break;
			}
		}
	}
	else {
		throw std::invalid_argument("The value cannot be less than 1.");
	}
}

void Game::disbandArmy(std::shared_ptr<User> user, int& countSoldiers)
{
	if (countSoldiers > 0) {
		for (int i = 0; i < this->towns.size(); ++i) {
			if (this->towns[i].getOwnTown() == user) {
				this->armies[i].merge(countSoldiers);
				break;
			}
		}
	}
	else {
		throw std::invalid_argument("The value cannot be less than 1.");
	}
}

std::string Game::getMapJSON()
{
	return this->gameMap->getMapJson();
}

void Game::tick()
{
	// start zero tick
	this->notifyUsersTick(); 
	this->gameController.control(this->tickCount, this->towns);
	std::this_thread::sleep_for(std::chrono::milliseconds(this->cooldownTick));
	// end zero tick

	while (this->active) {
		auto start = std::chrono::steady_clock::now();
		++tickCount;
		for (auto& town : towns) {
			town.TownTickProcessing();
		}

		std::sort(this->towns.rbegin(), this->towns.rend());

		this->notifyUsersTick();

		this->gameController.control(this->tickCount, this->towns);
		
		this->actionManager->doActions();

		if (this->gameController.isGameEnd()) {
			for (auto& town : this->towns) {
				town.getOwnTown()->setLocation(Location::MENU, "menu");
			}
			this->active = false;
			break;
		}

		auto end = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		std::this_thread::sleep_for(std::chrono::milliseconds(this->cooldownTick - duration.count()));
	}
}

void Game::createUUID()
{
	Poco::UUIDGenerator generator;
	this->uuid = generator.create().toString();
}

void Game::createTowns(std::vector<int> &idTowns, std::vector<std::shared_ptr<User>> &users)
{
	this->towns.reserve(users.size());
	this->armies.reserve(users.size());
	
	Economy townEconomy(this->gameSettings->economy.startBudget,
		this->gameSettings->economy.startIncome,
		this->gameSettings->economy.startMultiplier);
	Buildings townBuildings(this->gameSettings->buildings.priceIncrease);
	townBuildings.setPriceBuildings(0, this->gameSettings->buildings.priceChurch, this->gameSettings->buildings.modifierChurch); //church
	townBuildings.setPriceBuildings(1, this->gameSettings->buildings.priceManufactory, this->gameSettings->buildings.modifierManufactory); //manufactory

	for (int i = 0; i < users.size(); i++) {
		this->towns.emplace_back(i, users[i], idTowns[i], townEconomy, townBuildings);
		this->armies.emplace_back(0, idTowns[i]);
	}
}

void Game::notifyUsersTick()
{
	Poco::JSON::Object json;
	Poco::JSON::Array townsArr;
	json.set("tick", this->tickCount);
	json.set("uuid", this->uuid);
	for (int it = 0; it < this->towns.size(); ++it) {
		Poco::JSON::Object objJson;
		objJson.set("town_id", this->towns[it].getID());
		objJson.set("username", this->towns[it].getOwnTown()->getUsername());
		objJson.set("networth", this->towns[it].getTownEconomy().getNetWorth());
		objJson.set("soldiers", this->armies[it].getCount());
		objJson.set("node_army", this->armies[it].getNode());
		townsArr.add(objJson);
	}
	json.set("towns", townsArr);
	for (auto& town : this->towns) {
		Poco::JSON::Object userJson = json;
		Poco::JSON::Object objJson;
		objJson.set("town_id", town.getID());
		objJson.set("budget", town.getTownEconomy().getBudget());
		objJson.set("multiplier", town.getTownEconomy().getMultiplier());
		objJson.set("income", town.getTownEconomy().getTickIncome());
		objJson.set("charch_id", 0);
		objJson.set("charch_count", town.getTownBuildings().getCountBuildings(0));
		objJson.set("charch_price", town.getTownBuildings().getPriceBuildings(0));
		objJson.set("manufactory_id", 1);
		objJson.set("manufactory_count", town.getTownBuildings().getCountBuildings(1));
		objJson.set("manufactory_price", town.getTownBuildings().getPriceBuildings(1));
		userJson.set("town", objJson);
		std::ostringstream oss;
		userJson.stringify(oss);
		town.getOwnTown()->messagePool.pushBackMessage(oss.str());
	}
}

std::vector<int> Game::createMap(int countUser)
{
	auto sizeMap = DimensionMap::detect(countUser);
	auto posTowns = DimensionMap::placeTowns(countUser, sizeMap);
	this->gameMap = std::make_unique<GameMap>(sizeMap);
	return this->gameMap->placeTowns(posTowns);
}

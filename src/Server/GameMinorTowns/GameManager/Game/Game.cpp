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
	this->users = users;
	this->init();
	for (auto& user : users) {
		user->setLocation(Location::GAME, this->uuid);
		user->messagePool.pushBackMessage(this->gameMap->getMapJson());
	}
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
		if (town->getOwnTown() == user) {
			town->buildBuilding(buildingType);
			break;
		}
	}
}

void Game::tick()
{
	int cooldown = 5000;
	while (this->active) {
		auto start = std::chrono::steady_clock::now();
		++tickCount;
		for (auto& town : towns) {
			town->TownTickProcessing();
		}

		Poco::JSON::Object json;
		Poco::JSON::Array townsArr;
		json.set("tick", this->tickCount);
		json.set("uuid", this->uuid);
		for (int i = 0; i < this->towns.size(); ++i) {
			Poco::JSON::Object objJson;
			objJson.set("town_id", i);
			objJson.set("username", this->towns[i]->getOwnTown()->getUsername());
			objJson.set("networth", this->towns[i]->getTownEconomy().getNetWorth());
			townsArr.add(objJson);
		}
		json.set("towns", townsArr);
		for (int i = 0; i < this->users.size(); ++i) {
			Poco::JSON::Object userJson = json;
			Poco::JSON::Object objJson;
			objJson.set("town_id", i);
			objJson.set("budget", this->towns[i]->getTownEconomy().getBudget());
			objJson.set("multiplier", this->towns[i]->getTownEconomy().getMultiplier());
			objJson.set("income", this->towns[i]->getTownEconomy().getTickIncome());
			objJson.set("charch_id", 0);
			objJson.set("charch_count", this->towns[i]->getTownBuildings().getCountBuildings(0));
			objJson.set("charch_price", this->towns[i]->getTownBuildings().getPriceBuildings(0));
			objJson.set("manufactory_id", 1);
			objJson.set("manufactory_count", this->towns[i]->getTownBuildings().getCountBuildings(1));
			objJson.set("manufactory_price", this->towns[i]->getTownBuildings().getPriceBuildings(1));
			userJson.set("town", objJson);
			std::ostringstream oss;
			userJson.stringify(oss);
			this->users[i]->messagePool.pushBackMessage(oss.str());
		}

		auto end = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		std::this_thread::sleep_for(std::chrono::milliseconds(cooldown - duration.count()));
	}
}

void Game::init()
{
	this->createUUID();
	std::vector<int> idTowns = this->createMap();
	this->createTowns(idTowns);
}

void Game::createUUID()
{
	Poco::UUIDGenerator generator;
	this->uuid = generator.create().toString();
}

void Game::createTowns(std::vector<int> &idTowns)
{
	this->towns.reserve(this->users.size());
	for (int i = 0; i < users.size(); i++) {
		this->towns.push_back(std::make_unique<Town>(this->users[i], idTowns[i]));
	}
}

std::vector<int> Game::createMap()
{
	auto sizeMap = DimensionMap::detect(this->users.size());
	auto posTowns = DimensionMap::placeTowns(this->users.size(), sizeMap);
	this->gameMap = std::make_unique<GameMap>(sizeMap);
	return this->gameMap->placeTowns(posTowns);
}

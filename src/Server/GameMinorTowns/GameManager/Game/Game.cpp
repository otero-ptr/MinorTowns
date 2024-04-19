#include "Game.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/UUID.h"
#include "User/User.h"

Game::Game(std::vector<std::shared_ptr<User>> users)
{
	this->users = users;
	this->init();
	for (auto& user : users) {
		user->messagePool.pushBackMessage(this->gameMap->getMapJson());
	}
}

const std::string Game::getUUID()
{
	return this->uuid;
}

void Game::tick()
{

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

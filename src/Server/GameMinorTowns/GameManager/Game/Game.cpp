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
	this->createMap();
	this->createTowns();
	this->gameMap->createMapJson();
}

void Game::createUUID()
{
	Poco::UUIDGenerator generator;
	this->uuid = generator.create().toString();
}

void Game::createTowns()
{
	this->towns.reserve(this->users.size());
	for (auto it : users) {
		this->towns.push_back(std::make_unique<Town>(it, this->gameMap->getFreeSquarePosition()));
	}
}

void Game::createMap()
{
	this->gameMap = std::make_unique<GameMap>(5,5);
}

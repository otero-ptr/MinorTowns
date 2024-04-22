#include "GameManager.h"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::createGame(std::vector<std::shared_ptr<User>> users)
{
	std::unique_ptr<Game> game = std::make_unique<Game>(users);
	std::string uuid(game->getUUID());
	this->games.insert(std::make_pair(uuid, std::move(game)));
}

void GameManager::buildBuildings(std::shared_ptr<User> user, int& buildingType)
{
	this->games[user->getUUIDLocation()]->buildBuildings(user, buildingType);
}
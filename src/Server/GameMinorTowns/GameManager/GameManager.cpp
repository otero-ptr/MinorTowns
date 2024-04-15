#include "GameManager.h"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::createGame(std::vector<std::shared_ptr<User>> users)
{
	this->games.emplace(0, std::make_unique<Game>(users));
}

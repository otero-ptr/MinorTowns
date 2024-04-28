#include "GameManager.h"
#include <iostream>

GameManager::GameManager(int cooldownCollector)
{
	this->cooldownCollector = cooldownCollector;
	this->active = true;
	this->thCollector = std::jthread(&GameManager::CollectorEndedGames, this);
}

GameManager::~GameManager()
{
	this->active = false;
	if (this->thCollector.joinable()) {
		this->thCollector.join();
	}
}

void GameManager::createGame(std::vector<std::shared_ptr<User>> users)
{
	std::unique_ptr<Game> game = std::make_unique<Game>(users);

	for (auto& user : users) {
		user->setLocation(Location::GAME, game->getUUID());
		user->messagePool.pushBackMessage(game->getMapJSON());
	}
	this->games.insert(std::make_pair(game->getUUID(), std::move(game)));


}

void GameManager::buildBuildings(std::shared_ptr<User> user, int& buildingType)
{
	this->games[user->getUUIDLocation()]->buildBuildings(user, buildingType);
}

void GameManager::raiseArmy(std::shared_ptr<User> user, int& countSoldiers)
{
	this->games[user->getUUIDLocation()]->raiseArmy(user, countSoldiers);
}

void GameManager::disbandArmy(std::shared_ptr<User> user, int& countSoldiers)
{
	this->games[user->getUUIDLocation()]->disbandArmy(user, countSoldiers);
}

void GameManager::CollectorEndedGames()
{
	while (this->active) {
		for (auto it = this->games.begin(); it != this->games.end();)
		{
			if (!it->second->isActive())
			{
				it = this->games.erase(it);
			}
			else {
				++it;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(this->cooldownCollector));
	}
}

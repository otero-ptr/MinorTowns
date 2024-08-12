#include "GameManager.h"
#include <iostream>

GameManager::GameManager(int cooldown_collector) 
	: cooldown_collector(cooldown_collector),
	game_settings(std::make_shared<GameSettings>())
{
	this->th_collector = std::jthread(&GameManager::CollectorEndedGames, this);
}

GameManager::~GameManager()
{
	this->active = false;
	if (this->th_collector.joinable()) {
		this->th_collector.join();
	}
}

void GameManager::createGame(std::vector<std::shared_ptr<User>> users)
{
	std::unique_ptr<Game> game = std::make_unique<Game>(users, game_settings);
	for (auto& user : users) {
		user->setLocation(Location::GAME, game->getUUID());
	}
	game->prepare(game_settings);
	game->start();
	this->games.insert(std::make_pair(game->getUUID(), std::move(game)));
}

void GameManager::buildBuildings(std::shared_ptr<User> user, int& building_type)
{
	this->games[user->getUUIDLocation()]->buildBuildings(user, building_type);
}

void GameManager::raiseArmy(std::shared_ptr<User> user, int& count_soldiers)
{
	this->games[user->getUUIDLocation()]->raiseArmy(user, count_soldiers);
}

void GameManager::disbandArmy(std::shared_ptr<User> user, int& count_soldiers)
{
	this->games[user->getUUIDLocation()]->disbandArmy(user, count_soldiers);
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
		std::this_thread::sleep_for(std::chrono::seconds(this->cooldown_collector));
	}
}

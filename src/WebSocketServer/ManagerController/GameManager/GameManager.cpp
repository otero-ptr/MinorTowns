#include "GameManager.h"
#include "GameManager.h"
#include "Game/GameSettings/GameSettings.h"
#include "User.h"
#include "log.h"

GameManager::GameManager(int cooldown_collector) 
	: game_settings(std::make_shared<GameSettings>()),
	cooldown_collector(cooldown_collector),
	th_collector(std::jthread([this](std::stop_token token) { collectingEndedGames(token); }))
{
}

GameManager::~GameManager()
{
	if (th_collector.joinable()) {
		th_collector.request_stop();
		th_collector.join();
	}
}

void GameManager::createGame(std::vector<std::shared_ptr<User>>&& users)
{
	std::unique_ptr<Game> new_game = std::make_unique<Game>(users, game_settings);
	for (auto& user : users) {
		user->setLocation(Location::GAME, new_game->getUUID());
	}
	new_game->start();
	this->games.insert(std::make_pair(new_game->getUUID(), std::move(new_game)));
}

void GameManager::buildBuildings(std::shared_ptr<User>& user, int& building_type)
{
	auto it = games.find(user->getUUIDLocation());
	if (it != games.end()) {
		it->second->buildBuildings(user, building_type);
	}
}

void GameManager::raiseArmy(std::shared_ptr<User>& user, int& count_soldiers)
{
	auto it = games.find(user->getUUIDLocation());
	if (it != games.end()) {
		it->second->raiseArmy(user, count_soldiers);
	}
}

void GameManager::disbandArmy(std::shared_ptr<User>& user, int& count_soldiers)
{
	auto it = games.find(user->getUUIDLocation());
	if (it != games.end()) {
		it->second->disbandArmy(user, count_soldiers);
	}
}

void GameManager::leftGame(std::shared_ptr<User>& user)
{
	auto it = games.find(user->getUUIDLocation());
	if (it != games.end()) {
		it->second->defeated(user);
	}
}

void GameManager::moveArmy(std::shared_ptr<User>& user, int node) {
	auto it = games.find(user->getUUIDLocation());
	if (it != games.end()) {
		it->second->moveArmy(user, node);
	}
}

void GameManager::collectingEndedGames(std::stop_token token)
{
	while (!token.stop_requested()) {
		LOGGER_INFO("Collecting ended games")
		for (auto it = games.begin(); it != games.end();)
		{
			if (!it->second->isActive())
			{
				it = games.erase(it);
			}
			else {
				++it;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(cooldown_collector));
	}
}

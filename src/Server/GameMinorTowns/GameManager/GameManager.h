#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "Game/Game.h"

class User;

class GameManager {
public:
	GameManager() = delete;
	GameManager(int cooldownCollector);
	~GameManager();
	void createGame(std::vector<std::shared_ptr<User>> users);
	void getGame() {}
	void buildBuildings(std::shared_ptr<User> user, int& buildingType);
private:
	void CollectorEndedGames();
	std::unordered_map<std::string, std::unique_ptr<Game>> games;

	int cooldownCollector;
	bool active;
	std::jthread thCollector;
};
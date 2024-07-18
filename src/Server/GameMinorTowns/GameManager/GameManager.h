#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "Game/Game.h"

class User;

class GameManager {
public:
	GameManager() = delete;
	GameManager(int cooldown_collector);
	~GameManager();
	void createGame(std::vector<std::shared_ptr<User>> users);
	void getGame() {}
	void buildBuildings(std::shared_ptr<User> user, int& building_type);
	void raiseArmy(std::shared_ptr<User> user, int& count_soldiers);
	void disbandArmy(std::shared_ptr<User> user, int& count_soldiers);
private:
	void CollectorEndedGames();
	std::unordered_map<std::string, std::unique_ptr<Game>> games;

	int cooldown_collector;
	bool active = true;
	std::jthread th_collector;
};
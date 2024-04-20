#pragma once
#include <memory>
#include <vector>
#include <string>
#include "GameMap/GameMap.h"
#include "Town/Town.h"
#include <thread>

class User;

class Game {
public:
	Game() = delete;
	Game(std::vector<std::shared_ptr<User>> users);
	~Game();
	const std::string getUUID();
	bool isActive();
private:
	void tick();
	void init();
	void createUUID();
	void createTowns(std::vector<int>& idTowns);
	std::vector<int> createMap();

	std::vector<std::shared_ptr<User>> users;
	std::string uuid;

	std::unique_ptr<GameMap> gameMap;
	std::vector<std::unique_ptr<Town>> towns;

	bool active;
	std::jthread thTick;
	int tickCount = 0;
};
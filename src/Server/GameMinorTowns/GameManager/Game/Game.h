#pragma once
#include <memory>
#include <vector>
#include <string>
#include "GameMap/GameMap.h"
#include "Town/Town.h"
#include <thread>
#include <cstdarg>	

class User;

class Game {
public:
	Game() = delete;
	Game(std::vector<std::shared_ptr<User>> users);
	~Game();
	const std::string getUUID();
	bool isActive();
	void buildBuildings(std::shared_ptr<User> user, int &buildingType);
	std::string getMapJSON();
private:
	void tick();
	void createUUID();
	std::vector<int> createMap(int countUser);
	void createTowns(std::vector<int>& idTowns, std::vector<std::shared_ptr<User>>& users);
	
	void notifyUsers();

	std::vector<std::unique_ptr<Town>> towns;
	std::unique_ptr<GameMap> gameMap;

	bool active;
	std::jthread thTick;
	int tickCount = 0;
	std::string uuid;
};
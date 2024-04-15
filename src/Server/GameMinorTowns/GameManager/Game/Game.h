#pragma once
#include <memory>
#include <vector>
#include <string>
#include "GameMap/GameMap.h"
#include "Town/Town.h"

class User;

class Game {
public:
	Game() = delete;
	Game(std::vector<std::shared_ptr<User>> users);
	~Game() {}
	const std::string getUUID();
	void tick();
private:
	void init();
	void createUUID();
	void createTowns();
	void createMap();

	std::vector<std::shared_ptr<User>> users;
	std::string uuid;

	std::unique_ptr<GameMap> gameMap;
public:
	std::vector<std::unique_ptr<Town>> towns;
};
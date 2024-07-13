#pragma once
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include "GameMap/GameMap.h"
#include "Town/Town.h"
#include "Army/Army.h"
#include "Army/Battle/Battle.h"
#include "GameController/GameController.h"
#include "GameSettings/GameSettings.h"
#include "ActionManager/ActionManager.h"


class User;

class Game {
public:
	Game() = delete;
	Game(std::vector<std::shared_ptr<User>> users);
	~Game();
	const std::string getUUID();
	bool isActive();
	void buildBuildings(std::shared_ptr<User> user, int &buildingType);
	void raiseArmy(std::shared_ptr<User> user, int &countSoldiers);
	void disbandArmy(std::shared_ptr<User> user, int &countSoldiers);
	void attackArmy(std::shared_ptr<User> user);
	std::string getMapJSON();
private:
	void tick();
	void createUUID();
	std::vector<int> createMap(int countUser);
	void createTowns(std::vector<int>& idTowns, std::vector<std::shared_ptr<User>>& users);
	
	void notifyUsersTick();

	std::vector<Town> towns;

	std::vector<Army> armies;

	std::list<Battle> battles;

	std::unique_ptr<GameMap> gameMap;
	std::unique_ptr<ActionManager> actionManager;

	GameController gameController;

	GameSettings *gameSettings;

	int tickCount = 0;
	bool active;
	int cooldownTick;
	std::jthread thTick;

	std::string uuid;
};
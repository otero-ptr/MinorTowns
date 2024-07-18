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
	void buildBuildings(std::shared_ptr<User> user, int &building_type);
	void raiseArmy(std::shared_ptr<User> user, int &count_soldiers);
	void disbandArmy(std::shared_ptr<User> user, int &count_soldiers);
	void attackArmy(std::shared_ptr<User> user);
	std::string getMapJSON();
private:
	void tick();
	std::vector<int> createMap(int count_user);
	void createTowns(std::vector<int>& id_towns, std::vector<std::shared_ptr<User>>& users);
	
	void notifyUsersTick();

	std::string uuid;

	std::vector<Town> towns;

	std::vector<Army> armies;

	std::list<Battle> battles;

	std::unique_ptr<GameMap> game_map;
	std::unique_ptr<ActionManager> action_manager;

	GameController game_controller;

	std::unique_ptr<GameSettings> game_settings;

	int tick_count = 0;
	bool active = true;
	std::jthread th_tick;

};
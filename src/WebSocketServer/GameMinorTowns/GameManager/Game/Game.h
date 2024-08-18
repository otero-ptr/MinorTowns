#pragma once
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include "GameMap/GameMap.h"
#include "Town/Town.h"
#include "Army/Army.h"
#include "Army/Battle/Battle.h"
#include "GameSettings/GameSettings.h"
#include "GameController/GameController.h"
#include "nlohmann/json.hpp"

#include "spdlog/spdlog.h"

class User;

class Game {
public:
	Game() = delete;
	explicit Game(std::vector<std::shared_ptr<User>> users, std::shared_ptr<GameSettings> settings);
	~Game();
	void start();
	void prepare(std::shared_ptr<GameSettings> settings);
	const std::string getUUID();
	bool isActive();
	void buildBuildings(std::shared_ptr<User>& user, int &building_type);
	void raiseArmy(std::shared_ptr<User>& user, int &count_soldiers);
	void disbandArmy(std::shared_ptr<User>& user, int &count_soldiers);
	void attackArmy(std::shared_ptr<User>& user);
	void defeated(std::shared_ptr<User>& user);
private:
	void tick(std::stop_token token);
	std::vector<int> createMap(size_t count_user);
	void createTowns(std::vector<int>& id_towns, std::shared_ptr<GameSettings> settings);
	nlohmann::json makeCommonJson();
	nlohmann::json makeTableJson();
	nlohmann::json createTownJson(const TownData&& data, const nlohmann::json& common_json_obj);

	void notifyTick();
	void notifyTable();
	void notifyMap();

	GameController game_controller;
	std::string uuid;
	std::vector<Town> towns;
	std::vector<std::shared_ptr<User>> users;
	std::vector<Army> armies;
	std::list<Battle> battles;
	std::unique_ptr<GameMap> game_map;

	const int cooldown_tick;
	int tick_count = 0;
	std::jthread th_tick;

	std::unique_ptr<spdlog::logger> logger;

	std::mutex mtx;
};
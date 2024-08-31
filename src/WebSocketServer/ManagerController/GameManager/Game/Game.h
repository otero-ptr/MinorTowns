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

struct ArmyData {
	std::list<uint8_t> way;
	uint8_t node_id;
};

struct TownData {
	uint8_t node_id;
};

class Game {
	using Leaderboard = std::vector<std::pair<std::shared_ptr<User>, std::shared_ptr<Town>>>;
	using ArmyInfo = std::pair<ArmyData, std::shared_ptr<Army>>;
	using TownInfo = std::pair<TownData, std::shared_ptr<Town>>;
public:
	Game() = delete;
	explicit Game(std::vector<std::shared_ptr<User>> users, 
		std::shared_ptr<GameSettings> settings);
	~Game();
	void start();
	const std::string getUUID();
	bool isActive();
	void buildBuildings(std::shared_ptr<User>& user, int &building_type);
	void raiseArmy(std::shared_ptr<User>& user, uint32_t soldiers);
	void disbandArmy(std::shared_ptr<User>& user, uint32_t soldiers);
	void defeated(std::shared_ptr<User>& user);
	void moveArmy(std::shared_ptr<User>& user, int node);
private:
	void prepare(std::vector<std::shared_ptr<User>>& users, 
		std::shared_ptr<GameSettings> settings);
	std::vector<uint8_t> createMap(size_t count_user);
	void createGameData(std::vector<std::shared_ptr<User>>& users, 
		const std::vector<uint8_t>& id_towns, 
		std::shared_ptr<GameSettings> settings);
	void tick(std::stop_token token);
	nlohmann::json makeCommonJson();
	nlohmann::json createTownJson(const TownDetails&& data, 
		const nlohmann::json& common_json_obj);

	void notifyTick();
	void notifyMap();
	void armyProcessing();
	void leaderboardProcessing();
	void sortLeaderboard();

	std::string uuid;

	Leaderboard leaderboard;
	std::unordered_map<std::string, ArmyInfo>  armies;
	std::unordered_map<std::string, TownInfo> towns;

	std::list<Battle> battles;
	std::unique_ptr<GameMap> game_map;

	std::unique_ptr<spdlog::logger> logger;

	std::mutex mtx;
	GameController game_controller;

	std::jthread th_tick;
	const int cooldown_tick;
};
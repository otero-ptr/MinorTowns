#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "Game/Game.h"

class User;
class GameSettings;

class GameManager {
public:
	GameManager() = delete;
	explicit GameManager(int cooldown_collector);
	~GameManager();
	void createGame(std::vector<std::shared_ptr<User>>&& users);
	void getGame() {}
	void buildBuildings(std::shared_ptr<User>& user, int building_type);
	void raiseArmy(std::shared_ptr<User>& user, int32_t count_soldiers);
	void disbandArmy(std::shared_ptr<User>& user, int32_t count_soldiers);
	void leftGame(std::shared_ptr<User>& user);
	void moveArmy(std::shared_ptr<User>& user, uint8_t node);
private:
	void collectingEndedGames(std::stop_token token);
	std::shared_ptr<GameSettings> game_settings;
	std::unordered_map<std::string, std::unique_ptr<Game>> games;

	int cooldown_collector;
	std::jthread th_collector;
};
#pragma once
#include <memory>
#include <string>
#include "GameMinorTowns\GameManager\GameManager.h"
#include "GameMinorTowns\LobbyManager\LobbyManager.h"
class User;

class GameMinorTowns {
public:
	GameMinorTowns() = delete;
	GameMinorTowns(int max_users, int cooldown_collector, int cooldown_refresher, std::string redis_uri);
	void createLobby(std::shared_ptr<User> user,const int max_game_users);
	void joinLobby(std::shared_ptr<User> user,const std::string uuid_lobby);
	void leaveLobby(std::shared_ptr<User> user);
	void buildBuildings(std::shared_ptr<User> user, int building_type);
	void subscribeUpdateLobby(std::shared_ptr<User> user);
	void unsubscribeUpdateLobby(std::shared_ptr<User> user);

	void raiseArmy(std::shared_ptr<User> user, int count_soldiers);
	void disbandArmy(std::shared_ptr<User> user, int count_soldiers);
private:
	int max_users;
	std::unique_ptr<GameManager> game_manager;
	std::unique_ptr<LobbyManager> lobby_manager;
};
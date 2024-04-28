#pragma once
#include <memory>
#include <string>
#include "GameMinorTowns\GameManager\GameManager.h"
#include "GameMinorTowns\LobbyManager\LobbyManager.h"
class User;

class GameMinorTowns {
public:
	GameMinorTowns() = delete;
	GameMinorTowns(int maxUsers, int cooldownCollector, int cooldownRefresher);
	void createLobby(std::shared_ptr<User> user,const int maxGameUsers);
	void joinLobby(std::shared_ptr<User> user,const std::string uuidLobby);
	void leaveLobby(std::shared_ptr<User> user);
	void buildBuildings(std::shared_ptr<User> user, int buildingType);
	void subscribeUpdateLobby(std::shared_ptr<User> user);
	void unsubscribeUpdateLobby(std::shared_ptr<User> user);

	void raiseArmy(std::shared_ptr<User> user, int countSoldiers);
	void disbandArmy(std::shared_ptr<User> user, int countSoldiers);
private:
	int maxUsers;
	std::unique_ptr<GameManager> gameManager;
	std::unique_ptr<LobbyManager> lobbyManager;
};
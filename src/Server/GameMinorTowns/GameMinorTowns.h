#pragma once
#include <memory>
#include <string>
#include "GameMinorTowns\GameManager\GameManager.h"
#include "GameMinorTowns\LobbyManager\LobbyManager.h"
class User;

class GameMinorTowns {
public:
	GameMinorTowns();
	void createLobby(std::shared_ptr<User> user,const int maxUsers);
	void joinLobby(std::shared_ptr<User> user,const std::string uuidLobby);
	void leaveLobby(std::shared_ptr<User> user);
	void receiveLobbyList(std::shared_ptr<User> user);
	void buildBuildings(std::shared_ptr<User> user, int buildingType);
private:
	std::unique_ptr<GameManager> gameManager;
	std::unique_ptr<LobbyManager> lobbyManager;
};
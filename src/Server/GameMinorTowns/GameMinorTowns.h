#pragma once
#include <memory>
#include <string>
#include "GameMinorTowns\GameManager\GameManager.h"
#include "GameMinorTowns\LobbyManager\LobbyManager.h"
class User;

class GameMinorTowns {
public:
	GameMinorTowns();
	void createGame(std::shared_ptr<User> user, int maxUsers);
	void joinLobby(std::shared_ptr<User> user, std::string uuidLobby);
	void leaveLobby(std::shared_ptr<User> user);
	void receiveLobbyList(std::shared_ptr<User> user);
private:
	std::unique_ptr<GameManager> gameManager;
	std::unique_ptr<LobbyManager> lobbyManager;
};
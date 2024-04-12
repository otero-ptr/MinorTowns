#include "GameMinorTowns.h"
#include "User\User.h"

GameMinorTowns::GameMinorTowns()
{
	this->gameManager = std::make_unique<GameManager>();
	this->lobbyManager = std::make_unique<LobbyManager>();
}

void GameMinorTowns::createGame(std::shared_ptr<User> user, int maxUsers)
{
	std::string uuidLobby = this->lobbyManager->createLobby(maxUsers);
	this->lobbyManager->joinLobby(uuidLobby, user);
}

void GameMinorTowns::joinLobby(std::shared_ptr<User> user, std::string uuidLobby)
{
	this->lobbyManager->joinLobby(uuidLobby, user);
}

void GameMinorTowns::leaveLobby(std::shared_ptr<User> user)
{
	this->lobbyManager->leaveLobby(user);
}

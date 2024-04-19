#include "GameMinorTowns.h"
#include "User\User.h"

GameMinorTowns::GameMinorTowns()
{
	this->gameManager = std::make_unique<GameManager>();
	this->lobbyManager = std::make_unique<LobbyManager>();
}

void GameMinorTowns::createGame(std::shared_ptr<User> user,const int maxUsers)
{
	if (maxUsers > 4 || maxUsers < 2) {
		user->messagePool.pushBackMessage("only 2-4 users");
		return;
	}
	std::string uuidLobby = this->lobbyManager->createLobby(maxUsers);
	this->lobbyManager->joinLobby(uuidLobby, user);
}

void GameMinorTowns::joinLobby(std::shared_ptr<User> user,const std::string uuidLobby)
{
	this->lobbyManager->joinLobby(uuidLobby, user);
	if (this->lobbyManager->isLobbyFull(uuidLobby)) {
		this->gameManager->createGame(this->lobbyManager->getLobbyUsers(uuidLobby));
		this->lobbyManager->closeLobby(uuidLobby);
	}
}

void GameMinorTowns::leaveLobby(std::shared_ptr<User> user)
{
	this->lobbyManager->leaveLobby(user);
}

void GameMinorTowns::receiveLobbyList(std::shared_ptr<User> user)
{
	user->messagePool.pushBackMessage(this->lobbyManager->getListLobby());
}

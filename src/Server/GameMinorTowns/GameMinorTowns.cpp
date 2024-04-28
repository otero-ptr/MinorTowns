#include "GameMinorTowns.h"
#include "User\User.h"

GameMinorTowns::GameMinorTowns(int maxUsers, int cooldownCollector, int cooldownRefresher)
{
	this->maxUsers = maxUsers;
	this->gameManager = std::make_unique<GameManager>(cooldownCollector);
	this->lobbyManager = std::make_unique<LobbyManager>(cooldownRefresher);
}

void GameMinorTowns::createLobby(std::shared_ptr<User> user,const int maxGameUsers)
{
	if (maxGameUsers > this->maxUsers || maxGameUsers < 2) {
		return;
	}
	std::string uuidLobby = this->lobbyManager->createLobby(maxGameUsers);
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
	std::string uuidTemp = user->getUUIDLocation();
	this->lobbyManager->leaveLobby(user);
	if (this->lobbyManager->isLobbyEmpty(uuidTemp)) {
		this->lobbyManager->closeLobby(uuidTemp);
	}
}

void GameMinorTowns::buildBuildings(std::shared_ptr<User> user, int buildingType)
{
	this->gameManager->buildBuildings(user, buildingType);
}

void GameMinorTowns::subscribeUpdateLobby(std::shared_ptr<User> user)
{
	this->lobbyManager->notifier->subscribe(user);
}

void GameMinorTowns::unsubscribeUpdateLobby(std::shared_ptr<User> user)
{
	this->lobbyManager->notifier->unsubscribe(user);
}

void GameMinorTowns::raiseArmy(std::shared_ptr<User> user, int countSoldiers)
{
	this->gameManager->getGame();
}

void GameMinorTowns::disbandArmy(std::shared_ptr<User> user, int countSoldiers)
{
}

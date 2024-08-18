#include "GameMinorTowns.h"
#include "User.h"

GameMinorTowns::GameMinorTowns(int max_users, int cooldown_collector, int cooldown_refresher, std::string redis_uri) 
	: max_users(max_users),
	game_manager(std::make_unique<GameManager>(cooldown_collector)),
	lobby_manager(std::make_unique<LobbyManager>(cooldown_refresher, redis_uri))
{

}

void GameMinorTowns::createLobby(std::shared_ptr<User>& user,const int max_game_users)
{
	if (max_game_users > this->max_users || max_game_users < 2) {
		return;
	}
	std::string uuidLobby = this->lobby_manager->createLobby(max_game_users);
	this->lobby_manager->joinLobby(uuidLobby, user);
}

void GameMinorTowns::joinLobby(std::shared_ptr<User>& user,const std::string uuid_lobby)
{
	if (this->lobby_manager->isLobby(uuid_lobby)){
		this->lobby_manager->joinLobby(uuid_lobby, user);
		if (this->lobby_manager->isLobbyFull(uuid_lobby)) {
			this->game_manager->createGame(std::move(this->lobby_manager->extractLobbyUsers(uuid_lobby)));
			this->lobby_manager->closeLobby(uuid_lobby);
		}
	}
}

void GameMinorTowns::leaveLobby(std::shared_ptr<User>& user)
{
	std::string uuid_temp = user->getUUIDLocation();
	this->lobby_manager->leaveLobby(user);
	if (this->lobby_manager->isLobbyEmpty(uuid_temp)) {
		this->lobby_manager->closeLobby(uuid_temp);
	}
}

void GameMinorTowns::leftGame(std::shared_ptr<User>& user) {
	game_manager->leftGame(user);
}

void GameMinorTowns::buildBuildings(std::shared_ptr<User>& user, int buildingType)
{
	this->game_manager->buildBuildings(user, buildingType);
}

void GameMinorTowns::subscribeUpdateLobby(std::shared_ptr<User>& user)
{
	this->lobby_manager->subscribeLobbyUpdate(user);
}

void GameMinorTowns::unsubscribeUpdateLobby(std::shared_ptr<User>& user)
{
	this->lobby_manager->unsubscribeLobbyUpdate(user);
}

void GameMinorTowns::raiseArmy(std::shared_ptr<User>& user, int countSoldiers)
{
	
}

void GameMinorTowns::disbandArmy(std::shared_ptr<User>& user, int countSoldiers)
{
}

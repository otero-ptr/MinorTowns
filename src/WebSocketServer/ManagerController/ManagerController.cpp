#include "ManagerController.h"
#include "User.h"

ManagerController::ManagerController(int cooldown_collector, int cooldown_refresher, std::string redis_uri)
	: game_manager(std::make_unique<GameManager>(cooldown_collector)),
	lobby_manager(std::make_unique<LobbyManager>(cooldown_refresher, redis_uri))
{

}

std::optional<std::string> ManagerController::createLobby(std::shared_ptr<User>& user,const int max_game_users)
{
	std::string uuidLobby = this->lobby_manager->createLobby(max_game_users);
	lobby_manager->joinLobby(uuidLobby, user);
	return std::nullopt;
}

std::optional<std::string> ManagerController::joinLobby(std::shared_ptr<User>& user,const std::string uuid_lobby)
{
	std::lock_guard<std::mutex> lock(lobby_mtx);
	if (!lobby_manager->isLobby(uuid_lobby)){
		return "lobby doesn't exist";
	}
	if (lobby_manager->isLobbyFull(uuid_lobby)) {
		return "the lobby is full";
	}
	else {
		lobby_manager->joinLobby(uuid_lobby, user);
		if (lobby_manager->isLobbyFull(uuid_lobby)) {
			game_manager->createGame(std::move(lobby_manager->extractLobbyUsers(uuid_lobby)));
			lobby_manager->closeLobby(uuid_lobby);
		}
	}
	return std::nullopt;
}

std::optional<std::string> ManagerController::leaveLobby(std::shared_ptr<User>& user)
{
	std::lock_guard<std::mutex> lock(lobby_mtx);
	std::string uuid_temp = user->getUUIDLocation();
	lobby_manager->leaveLobby(user);
	if (lobby_manager->isLobbyEmpty(uuid_temp)) {
		lobby_manager->closeLobby(uuid_temp);
	}
	return std::nullopt;
}

std::optional<std::string> ManagerController::leftGame(std::shared_ptr<User>& user) {
	game_manager->leftGame(user);
	return std::nullopt;
}

std::optional<std::string> ManagerController::buildBuildings(std::shared_ptr<User>& user, int buildingType)
{
	game_manager->buildBuildings(user, buildingType);
	return std::nullopt;
}

std::optional<std::string> ManagerController::subscribeUpdateLobby(std::shared_ptr<User>& user)
{
	lobby_manager->subscribeLobbyUpdate(user);
	return std::nullopt;
}

std::optional<std::string> ManagerController::unsubscribeUpdateLobby(std::shared_ptr<User>& user)
{
	lobby_manager->unsubscribeLobbyUpdate(user);
	return std::nullopt;
}

std::optional<std::string> ManagerController::raiseArmy(std::shared_ptr<User>& user, int countSoldiers)
{
	return std::nullopt;
}

std::optional<std::string> ManagerController::disbandArmy(std::shared_ptr<User>& user, int countSoldiers)
{
	return std::nullopt;
}

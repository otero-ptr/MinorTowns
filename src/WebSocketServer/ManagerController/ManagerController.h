#pragma once
#include <memory>
#include <string>
#include "IManagerController.h"
#include "GameManager/GameManager.h"
#include "LobbyManager/LobbyManager.h"

class ManagerController : public IManagerController {
public:
	ManagerController() = delete;
	explicit ManagerController(int cooldown_collector, int cooldown_refresher, std::string redis_uri);
	std::optional<std::string> createLobby(std::shared_ptr<User> user,const int max_game_users) override;
	std::optional<std::string> joinLobby(std::shared_ptr<User> user,const std::string uuid_lobby) override;
	std::optional<std::string> leaveLobby(std::shared_ptr<User> user) override;
	std::optional<std::string> leftGame(std::shared_ptr<User> user) override;
	std::optional<std::string> buildBuildings(std::shared_ptr<User> user, int building_type) override;
	std::optional<std::string> subscribeUpdateLobby(std::shared_ptr<User> user) override;
	std::optional<std::string> unsubscribeUpdateLobby(std::shared_ptr<User> user) override;
	std::optional<std::string> raiseArmy(std::shared_ptr<User> user, int count_soldiers) override;
	std::optional<std::string> disbandArmy(std::shared_ptr<User> user, int count_soldiers) override;
	std::optional<std::string> moveArmy(std::shared_ptr<User> user, int node) override;
private:
	std::mutex lobby_mtx;
	std::unique_ptr<GameManager> game_manager;
	std::unique_ptr<LobbyManager> lobby_manager;
};
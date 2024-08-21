#pragma once
#include <memory>
#include <string>
#include <optional>

class User;

class IManagerController {
public:
	virtual std::optional<std::string> createLobby(std::shared_ptr<User>& user, const int max_game_users) = 0;
	virtual std::optional<std::string> joinLobby(std::shared_ptr<User>& user, const std::string uuid_lobby) = 0;
	virtual std::optional<std::string> leaveLobby(std::shared_ptr<User>& user) = 0;
	virtual std::optional<std::string> leftGame(std::shared_ptr<User>& user) = 0;
	virtual std::optional<std::string> buildBuildings(std::shared_ptr<User>& user, int building_type) = 0;
	virtual std::optional<std::string> subscribeUpdateLobby(std::shared_ptr<User>& user) = 0;
	virtual std::optional<std::string> unsubscribeUpdateLobby(std::shared_ptr<User>& user) = 0;
	virtual std::optional<std::string> raiseArmy(std::shared_ptr<User>& user, int count_soldiers) = 0;
	virtual std::optional<std::string> disbandArmy(std::shared_ptr<User>& user, int count_soldiers) = 0;
	virtual ~IManagerController() {}
};
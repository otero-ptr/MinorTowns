#pragma once
#include "gmock/gmock.h"
#include "IManagerController.h"
#include <memory>

class User;

class MockManagerController : public IManagerController {
public:
	MOCK_METHOD(std::optional<std::string>, createLobby, (std::shared_ptr<User>& user, const int max_game_users), (override));
	MOCK_METHOD(std::optional<std::string>, joinLobby, (std::shared_ptr<User>& user, const std::string uuid_lobby), (override));
	MOCK_METHOD(std::optional<std::string>, leaveLobby, (std::shared_ptr<User>& user), (override));
	MOCK_METHOD(std::optional<std::string>, leftGame, (std::shared_ptr<User>& user), (override));
	MOCK_METHOD(std::optional<std::string>, buildBuildings, (std::shared_ptr<User>& user, int building_type), (override));
	MOCK_METHOD(std::optional<std::string>, subscribeUpdateLobby, (std::shared_ptr<User>& user), (override));
	MOCK_METHOD(std::optional<std::string>, unsubscribeUpdateLobby, (std::shared_ptr<User>& user), (override));
	MOCK_METHOD(std::optional<std::string>, raiseArmy, (std::shared_ptr<User>& user, int count_soldiers), (override));
	MOCK_METHOD(std::optional<std::string>, disbandArmy, (std::shared_ptr<User>& user, int count_soldiers), (override));
};

#include <gtest\gtest.h>
#include "mock_manager_controller.h"
#include "Middleware.h"
#include "AuthorizationValidate\AuthorizationValidate.h"
#include "User.h"
#include "RequestResult.h"

TEST(MiddlewareTest, CheckUsername—orrect) {
	EXPECT_TRUE(AuthValidate::checkUsername("User5"));
	EXPECT_TRUE(AuthValidate::checkUsername("User123456789"));
	EXPECT_TRUE(AuthValidate::checkUsername("User_best"));
	EXPECT_TRUE(AuthValidate::checkUsername("User123"));
}

TEST(MiddlewareTest, CheckUsernameIncorrect) {
	EXPECT_FALSE(AuthValidate::checkUsername("User"));
	EXPECT_FALSE(AuthValidate::checkUsername("User1234567890"));
	EXPECT_FALSE(AuthValidate::checkUsername("User***"));
	EXPECT_FALSE(AuthValidate::checkUsername("User-User"));
	EXPECT_FALSE(AuthValidate::checkUsername("User."));
}

TEST(MiddlewareTest, AuthorizationUser) {
	std::shared_ptr<MockManagerController> mock = std::make_shared<MockManagerController>();
	Middleware middleware(mock);
	auto user = middleware.authorization("User5", "127.0.0.1", 7777);
	EXPECT_NE(user, nullptr);
	user = middleware.authorization("User", "127.0.0.1", 7777);
	EXPECT_EQ(user, nullptr);
}

TEST(MiddlewareTest, DisconnectUser) {
	std::shared_ptr<MockManagerController> mock = std::make_shared<MockManagerController>();
	Middleware middleware(mock);
	auto user = middleware.authorization("User5", "127.0.0.1", 7777);

	{
		user->setLocation(Location::MENU, "menu");
		EXPECT_CALL(*mock, unsubscribeUpdateLobby(user)).Times(1).WillOnce(::testing::Return(std::nullopt));
		EXPECT_EQ(middleware.disconnect(user), MIDDLEWARE_STATUS::ST_OK);
	}

	{
		user->setLocation(Location::LOBBY, "lobby_uuid");
		EXPECT_CALL(*mock, leaveLobby(user)).Times(1).WillOnce(::testing::Return(std::nullopt));
		EXPECT_EQ(middleware.disconnect(user), MIDDLEWARE_STATUS::ST_OK);
	}

	{
		user->setLocation(Location::GAME, "game_uuid");
		EXPECT_CALL(*mock, leftGame(user)).Times(1).WillOnce(::testing::Return(std::nullopt));
		EXPECT_EQ(middleware.disconnect(user), MIDDLEWARE_STATUS::ST_OK);
	}
}

TEST(MiddlewareTest, ActionCreateLobby) {
    const int max_users = 3;
	std::shared_ptr<MockManagerController> mock = std::make_shared<MockManagerController>();
	Middleware middleware(mock);
    auto user = middleware.authorization("User5", "127.0.0.1", 7777);

    {
        std::shared_ptr params = std::make_shared<Params::CreateLobby>();
        params->max_users = max_users;
        RequestResult request(RequestOperation::CREATE_LOBBY, params);
        EXPECT_CALL(*mock, createLobby(user, max_users)).Times(1).WillOnce(::testing::Return(std::nullopt));
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_OK);
        EXPECT_TRUE(result.second.empty());
    }

    {
        RequestResult request(RequestOperation::CREATE_LOBBY);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "missing params");
    }

    {
        std::shared_ptr params = std::make_shared<Params::CreateLobby>();
        params->max_users = max_users;
        RequestResult request(RequestOperation::CREATE_LOBBY, params);
        EXPECT_CALL(*mock, createLobby(user, max_users)).Times(1).WillOnce(::testing::Return("error"));
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "error");
    }
}

TEST(MiddlewareTest, ActionJoinLobby) {
    const std::string uuid = "uuid";
    std::shared_ptr<MockManagerController> mock = std::make_shared<MockManagerController>();
    Middleware middleware(mock);
    auto user = middleware.authorization("User5", "127.0.0.1", 7777);

    {
        std::shared_ptr params = std::make_shared<Params::JoinLobby>();
        params->uuid_lobby = uuid;
        RequestResult request(RequestOperation::JOIN_LOBBY, params);
        EXPECT_CALL(*mock, joinLobby(user, uuid)).Times(1).WillOnce(::testing::Return(std::nullopt));
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_OK);
        EXPECT_TRUE(result.second.empty());
    }

    {
        RequestResult request(RequestOperation::JOIN_LOBBY);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "missing params");
    }

    {
        std::shared_ptr params = std::make_shared<Params::JoinLobby>();
        params->uuid_lobby = uuid;
        RequestResult request(RequestOperation::JOIN_LOBBY, params);
        EXPECT_CALL(*mock, joinLobby(user, params->uuid_lobby)).Times(1).WillOnce(::testing::Return("error"));
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "error");
    }
}

TEST(MiddlewareTest, ActionLeaveLobby) {
    std::shared_ptr<MockManagerController> mock = std::make_shared<MockManagerController>();
    Middleware middleware(mock);
    auto user = middleware.authorization("User5", "127.0.0.1", 7777);
    user->setLocation(Location::LOBBY, "uuid");

    {
        RequestResult request(RequestOperation::LEAVE_LOBBY);
        EXPECT_CALL(*mock, leaveLobby(user)).Times(1).WillOnce(::testing::Return(std::nullopt));
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_OK);
        EXPECT_TRUE(result.second.empty());
    }

    {
        RequestResult request(RequestOperation::LEAVE_LOBBY);
        EXPECT_CALL(*mock, leaveLobby(user)).Times(1).WillOnce(::testing::Return("error"));
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "error");
    }
}

TEST(MiddlewareTest, ActionSubscribeUpdate) {
    std::shared_ptr<MockManagerController> mock = std::make_shared<MockManagerController>();
    Middleware middleware(mock);
    auto user = middleware.authorization("User5", "127.0.0.1", 7777);

    {
        RequestResult request(RequestOperation::SUBSCRIBE_UPDATE);
        EXPECT_CALL(*mock, subscribeUpdateLobby(user)).Times(1).WillOnce(::testing::Return(std::nullopt));
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_OK);
        EXPECT_TRUE(result.second.empty());
    }

    {
        RequestResult request(RequestOperation::SUBSCRIBE_UPDATE);
        EXPECT_CALL(*mock, subscribeUpdateLobby(user)).Times(1).WillOnce(::testing::Return("error"));
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "error");
    }
}

TEST(MiddlewareTest, ActionUnsubscribeUpdate) {
    std::shared_ptr<MockManagerController> mock = std::make_shared<MockManagerController>();
    Middleware middleware(mock);
    auto user = middleware.authorization("User5", "127.0.0.1", 7777);

    {
        RequestResult request(RequestOperation::UNSUBSCRIBE_UPDATE);
        EXPECT_CALL(*mock, unsubscribeUpdateLobby(user)).Times(1).WillOnce(::testing::Return(std::nullopt));
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_OK);
        EXPECT_TRUE(result.second.empty());
    }

    {
        RequestResult request(RequestOperation::UNSUBSCRIBE_UPDATE);
        EXPECT_CALL(*mock, unsubscribeUpdateLobby(user)).Times(1).WillOnce(::testing::Return("error"));
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "error");
    }
}

TEST(MiddlewareTest, ActionBuildBuildings) {
    const int building_id = 0;
    std::shared_ptr<MockManagerController> mock = std::make_shared<MockManagerController>();
    Middleware middleware(mock);
    auto user = middleware.authorization("User5", "127.0.0.1", 7777);
    user->setLocation(Location::GAME, "uuid");

    {
        std::shared_ptr params = std::make_shared<Params::BuildBuildings>();
        params->building_id = building_id;
        RequestResult request(RequestOperation::BUILD_BUILDINGS, params);
        EXPECT_CALL(*mock, buildBuildings(user, building_id)).Times(1).WillOnce(::testing::Return(std::nullopt));
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_OK);
        EXPECT_TRUE(result.second.empty());
    }

    {
        RequestResult request(RequestOperation::BUILD_BUILDINGS);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "missing params");
    }

    {
        std::shared_ptr params = std::make_shared<Params::BuildBuildings>();
        params->building_id = building_id;
        RequestResult request(RequestOperation::BUILD_BUILDINGS, params);
        EXPECT_CALL(*mock, buildBuildings(user, params->building_id)).Times(1).WillOnce(::testing::Return("error"));
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "error");
    }
}

TEST(MiddlewareTest, ActionRaiseArmy) {
    const int soldiers = 1000;
    std::shared_ptr<MockManagerController> mock = std::make_shared<MockManagerController>();
    Middleware middleware(mock);
    auto user = middleware.authorization("User5", "127.0.0.1", 7777);
    user->setLocation(Location::GAME, "uuid");

    {
        std::shared_ptr params = std::make_shared<Params::RaiseArmy>();
        params->soldiers = soldiers;
        RequestResult request(RequestOperation::RAISE_ARMY, params);
        EXPECT_CALL(*mock, raiseArmy(user, soldiers)).Times(1).WillOnce(::testing::Return(std::nullopt));
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_OK);
        EXPECT_TRUE(result.second.empty());
    }

    {
        RequestResult request(RequestOperation::RAISE_ARMY);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "missing params");
    }

    {
        std::shared_ptr params = std::make_shared<Params::RaiseArmy>();
        params->soldiers = soldiers;
        RequestResult request(RequestOperation::RAISE_ARMY, params);
        EXPECT_CALL(*mock, raiseArmy(user, soldiers)).Times(1).WillOnce(::testing::Return("error"));
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "error");
    }
}

TEST(MiddlewareTest, ActionDisbandArmy) {
    const int soldiers = 1000;
    std::shared_ptr<MockManagerController> mock = std::make_shared<MockManagerController>();
    Middleware middleware(mock);
    auto user = middleware.authorization("User5", "127.0.0.1", 7777);
    user->setLocation(Location::GAME, "uuid");

    {
        std::shared_ptr params = std::make_shared<Params::DisbandArmy>();
        params->soldiers = soldiers;
        RequestResult request(RequestOperation::DISBAND_ARMY, params);
        EXPECT_CALL(*mock, disbandArmy(user, soldiers)).Times(1).WillOnce(::testing::Return(std::nullopt));
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_OK);
        EXPECT_TRUE(result.second.empty());
    }

    {
        RequestResult request(RequestOperation::DISBAND_ARMY);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "missing params");
    }

    {
        std::shared_ptr params = std::make_shared<Params::DisbandArmy>();
        params->soldiers = soldiers;
        RequestResult request(RequestOperation::DISBAND_ARMY, params);
        EXPECT_CALL(*mock, disbandArmy(user, soldiers)).Times(1).WillOnce(::testing::Return("error"));
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "error");
    }
}

TEST(MiddlewareTest, WrongAction) {
    std::shared_ptr<MockManagerController> mock = std::make_shared<MockManagerController>();
    Middleware middleware(mock);
    auto user = middleware.authorization("User5", "127.0.0.1", 7777);
    std::shared_ptr params = std::make_shared<Params::Params>();

    user->setLocation(Location::MENU, "menu");
    { // wrong location
        RequestResult request(RequestOperation::LEAVE_LOBBY, params);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "unknown action");
    }

    { // wrong location
        RequestResult request(RequestOperation::BUILD_BUILDINGS, params);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "unknown action");
    }

    { // wrong location
        RequestResult request(RequestOperation::RAISE_ARMY, params);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "unknown action");
    }

    { // wrong location
        RequestResult request(RequestOperation::DISBAND_ARMY, params);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "unknown action");
    }
    user->setLocation(Location::GAME, "uuid");
    { // wrong location
        RequestResult request(RequestOperation::CREATE_LOBBY, params);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "unknown action");
    }

    { // wrong location
        RequestResult request(RequestOperation::JOIN_LOBBY, params);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "unknown action");
    }

    { // wrong location
        RequestResult request(RequestOperation::LEAVE_LOBBY, params);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "unknown action");
    }

    { // wrong location
        RequestResult request(RequestOperation::SUBSCRIBE_UPDATE, params);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "unknown action");
    }

    { // wrong location
        RequestResult request(RequestOperation::UNSUBSCRIBE_UPDATE, params);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "unknown action");
    }

    user->setLocation(Location::LOBBY, "uuid");
    { // wrong location
        RequestResult request(RequestOperation::CREATE_LOBBY, params);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "unknown action");
    }

    { // wrong location
        RequestResult request(RequestOperation::JOIN_LOBBY, params);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "unknown action");
    }

    { // wrong location
        RequestResult request(RequestOperation::SUBSCRIBE_UPDATE, params);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "unknown action");
    }

    { // wrong location
        RequestResult request(RequestOperation::UNSUBSCRIBE_UPDATE, params);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "unknown action");
    }

    { // wrong location
        RequestResult request(RequestOperation::BUILD_BUILDINGS, params);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "unknown action");
    }

    { // wrong location
        RequestResult request(RequestOperation::RAISE_ARMY, params);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "unknown action");
    }

    { // wrong location
        RequestResult request(RequestOperation::DISBAND_ARMY, params);
        auto result = middleware.action(std::move(request), user);
        EXPECT_EQ(result.first, MIDDLEWARE_STATUS::ST_ERROR);
        EXPECT_EQ(result.second, "unknown action");
    }
}
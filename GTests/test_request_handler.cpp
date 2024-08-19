#include <gtest\gtest.h>
#include "RequestHandler.h"
#include <iostream>

TEST(RequestHandlerTest, CreateLobby) {
    std::string request = "{ \"action\": \"create_lobby\", \"params\" : { \"max_users\": 3}}";
    auto result = RequestHandler::Handler(request);
    ASSERT_FALSE(std::holds_alternative<RequestError>(result));
    ASSERT_TRUE(std::holds_alternative<RequestResult>(result));
    RequestResult &value = std::get<RequestResult>(result);
    EXPECT_EQ(value.getOperation(), RequestOperation::CREATE_LOBBY);
    EXPECT_TRUE(value.isParams());
    ASSERT_NE(value.getParams().lock(), nullptr);
    auto params = std::static_pointer_cast<Params::CreateLobby>(value.getParams().lock());
    EXPECT_EQ(params->max_users, 3);
}

TEST(RequestHandlerTest, JoinLobby) {
    std::string request = "{\"action\": \"join_lobby\",\"params\" : { \"uuid\": \"5853f2b4-5e1c-11ef-96e1-0250cf951982\" } }";
    auto result = RequestHandler::Handler(request);
    ASSERT_FALSE(std::holds_alternative<RequestError>(result));
    ASSERT_TRUE(std::holds_alternative<RequestResult>(result));
    RequestResult& value = std::get<RequestResult>(result);
    EXPECT_EQ(value.getOperation(), RequestOperation::JOIN_LOBBY);
    EXPECT_TRUE(value.isParams());
    auto params = std::static_pointer_cast<Params::JoinLobby>(value.getParams().lock());
    EXPECT_EQ(params->uuid_lobby, "5853f2b4-5e1c-11ef-96e1-0250cf951982");
}

TEST(RequestHandlerTest, LeaveLobby) {
    std::string request = "{\"action\": \"leave_lobby\"}";
    auto result = RequestHandler::Handler(request);
    ASSERT_FALSE(std::holds_alternative<RequestError>(result));
    ASSERT_TRUE(std::holds_alternative<RequestResult>(result));
    RequestResult& value = std::get<RequestResult>(result);
    EXPECT_EQ(value.getOperation(), RequestOperation::LEAVE_LOBBY);
    EXPECT_FALSE(value.isParams());
}

TEST(RequestHandlerTest, SubscribeUpdateLobby) {
    std::string request = "{ \"action\": \"subscribe_update_lobby\" }";
    auto result = RequestHandler::Handler(request);
    ASSERT_FALSE(std::holds_alternative<RequestError>(result));
    ASSERT_TRUE(std::holds_alternative<RequestResult>(result));
    RequestResult& value = std::get<RequestResult>(result);
    EXPECT_EQ(value.getOperation(), RequestOperation::SUBSCRIBE_UPDATE);
    EXPECT_FALSE(value.isParams());
}

TEST(RequestHandlerTest, UnsubscribeUpdateLobby) {
    std::string request = "{ \"action\": \"unsubscribe_update_lobby\" }";
    auto result = RequestHandler::Handler(request);
    ASSERT_FALSE(std::holds_alternative<RequestError>(result));
    ASSERT_TRUE(std::holds_alternative<RequestResult>(result));
    RequestResult& value = std::get<RequestResult>(result);
    EXPECT_EQ(value.getOperation(), RequestOperation::UNSUBSCRIBE_UPDATE);
    EXPECT_FALSE(value.isParams());
}

TEST(RequestHandlerTest, BuildBuildings) {
    std::string request = "{\"action\": \"build_buildings\",\"params\" : {\"building_id\" : 0 } }";
    auto result = RequestHandler::Handler(request);
    ASSERT_FALSE(std::holds_alternative<RequestError>(result));
    ASSERT_TRUE(std::holds_alternative<RequestResult>(result));
    RequestResult& value = std::get<RequestResult>(result);
    auto params = std::static_pointer_cast<Params::BuildBuildings>(value.getParams().lock());
    EXPECT_EQ(params->building_id, 0);
}

TEST(RequestHandlerTest, RaiseArmy) {
    std::string request = "{\"action\": \"raise_army\",\"params\" : {\"soldiers\": 1000 } }";
    auto result = RequestHandler::Handler(request);
    ASSERT_FALSE(std::holds_alternative<RequestError>(result));
    ASSERT_TRUE(std::holds_alternative<RequestResult>(result));
    RequestResult& value = std::get<RequestResult>(result);
    EXPECT_EQ(value.getOperation(), RequestOperation::RAISE_ARMY);
    EXPECT_TRUE(value.isParams());
    auto params = std::static_pointer_cast<Params::RaiseArmy>(value.getParams().lock());
    EXPECT_EQ(params->soldiers, 1000);
}

TEST(RequestHandlerTest, DisbandArmy) {
    std::string request = "{\"action\": \"disband_army\",\"params\" : {\"soldiers\": 1000 } }";
    auto result = RequestHandler::Handler(request);
    ASSERT_FALSE(std::holds_alternative<RequestError>(result));
    ASSERT_TRUE(std::holds_alternative<RequestResult>(result));
    RequestResult& value = std::get<RequestResult>(result);
    EXPECT_EQ(value.getOperation(), RequestOperation::DISBAND_ARMY);
    EXPECT_TRUE(value.isParams());
    auto params = std::static_pointer_cast<Params::DisbandArmy>(value.getParams().lock());
    EXPECT_EQ(params->soldiers, 1000);
}

TEST(RequestHandlerTest, WrongAction) {
    std::string request = "{\"action\": \"join_loby\",\"params\" : { \"uuid\": \"5853f2b4-5e1c-11ef-96e1-0250cf951982\" } }";
    auto result = RequestHandler::Handler(request);
    ASSERT_FALSE(std::holds_alternative<RequestResult>(result));
    ASSERT_TRUE(std::holds_alternative<RequestError>(result));
    RequestError& value = std::get<RequestError>(result);
    ASSERT_EQ(value.err, "Action doesn't exist");
}

TEST(RequestHandlerTest, Wrong) {
    {
        std::string request = "{\"acion\": \"join_lobby\",\"params\" : { \"uuid\": \"5853f2b4-5e1c-11ef-96e1-0250cf951982\" } }";
        auto result = RequestHandler::Handler(request);
        ASSERT_FALSE(std::holds_alternative<RequestResult>(result));
        ASSERT_TRUE(std::holds_alternative<RequestError>(result));
        RequestError& value = std::get<RequestError>(result);
        ASSERT_EQ(value.err, "Action not specified");
    }

    {
        std::string request = "{ }";
        auto result = RequestHandler::Handler(request);
        ASSERT_FALSE(std::holds_alternative<RequestResult>(result));
        ASSERT_TRUE(std::holds_alternative<RequestError>(result));
        RequestError& value = std::get<RequestError>(result);
        ASSERT_EQ(value.err, "Action not specified");
    }

    {
        std::string request = "{ ";
        auto result = RequestHandler::Handler(request);
        ASSERT_FALSE(std::holds_alternative<RequestResult>(result));
        ASSERT_TRUE(std::holds_alternative<RequestError>(result));
    }

    {
        std::string request = "{{{dawd} ";
        auto result = RequestHandler::Handler(request);
        ASSERT_FALSE(std::holds_alternative<RequestResult>(result));
        ASSERT_TRUE(std::holds_alternative<RequestError>(result));
    }
}
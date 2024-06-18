#include "Middleware.h"
#include "GameMinorTowns/GameMinorTowns.h"
#include <thread>
#include "nlohmann\json.hpp"
#include <iostream>
#include <Poco\Exception.h>

Middleware::Middleware(std::shared_ptr<GameMinorTowns> game)
{
    this->gameMinorTowns = game;
}

Middleware::~Middleware()
{
}

std::shared_ptr<User> Middleware::authorization(std::string username, std::string address, unsigned short port)
{
    if (this->authValidate.checkUsername(username)) {
        return nullptr;
    }
	std::shared_ptr<User> user;
	user = std::make_shared<User>(username, address + ":" + std::to_string(port));
	return user;
}

MIDDLEWARE_STATUS Middleware::action(std::string jsonMessage, std::shared_ptr<User> user)
{
    try {
        nlohmann::json jsonObj = nlohmann::json::parse(jsonMessage);
        if (jsonObj.contains("action")) {
            std::cout << "Value of action: " << jsonObj["action"] << std::endl;
            if (jsonObj["action"].get<std::string>() == "create_game" && user->getLocation() == Location::MENU) {
                if (jsonObj.contains("params")) {
                    if (jsonObj["params"].contains("max_users")) {
                        int maxUsers = jsonObj["params"]["max_users"].get<int>();
                        if (maxUsers > 0) {
                            std::thread th(&GameMinorTowns::createLobby, this->gameMinorTowns, user, maxUsers);
                            th.detach();
                            return MIDDLEWARE_STATUS::ST_OK;
                        }
                    }
                }
                user->messagePool.pushBackMessage("{\"err\": \"param problem\"}");
                return MIDDLEWARE_STATUS::ST_ERROR;
            }
            else if (jsonObj["action"] == "join_lobby" && user->getLocation() == Location::MENU) {
                if (jsonObj.contains("params")) {
                    if (jsonObj["params"].contains("uuid")) {
                        std::string uuid = jsonObj["params"]["uuid"].get<std::string>();
                        if (!uuid.empty()) {
                            std::thread th(&GameMinorTowns::joinLobby, this->gameMinorTowns, user, uuid);
                            th.detach();
                            return MIDDLEWARE_STATUS::ST_OK;
                        }
                    }
                    user->messagePool.pushBackMessage("{\"err\": \"param problem\"}");
                    return MIDDLEWARE_STATUS::ST_ERROR;
                }
            } else if (jsonObj["action"] == "leave_lobby" && user->getLocation() == Location::LOBBY) {
                    std::thread th(&GameMinorTowns::leaveLobby, this->gameMinorTowns, user);
                    th.detach();
                    return MIDDLEWARE_STATUS::ST_OK;
            } else if (jsonObj["action"] == "subscribe_update_lobby" && user->getLocation() == Location::MENU) {
                    std::thread th(&GameMinorTowns::subscribeUpdateLobby, this->gameMinorTowns, user);
                    th.detach();
                    return MIDDLEWARE_STATUS::ST_OK;
            } else if (jsonObj["action"] == "unsubscribe_update_lobby" && user->getLocation() == Location::MENU) {
                    std::thread th(&GameMinorTowns::unsubscribeUpdateLobby, this->gameMinorTowns, user);
                    th.detach();
                    return MIDDLEWARE_STATUS::ST_OK;
            } else if (jsonObj["action"] == "build_buildings" && user->getLocation() == Location::GAME) {
                    if (jsonObj.contains("params")) {
                        if (jsonObj["params"].contains("building_id")) {
                            std::thread th(&GameMinorTowns::buildBuildings, this->gameMinorTowns, user, jsonObj["params"]["building_id"].get<int>());
                            th.detach();
                            return MIDDLEWARE_STATUS::ST_OK;
                        }
                    }
                    user->messagePool.pushBackMessage("{\"err\": \"param problem\"}");
                    return MIDDLEWARE_STATUS::ST_ERROR;
            } else if (jsonObj["action"] == "raise_army" && user->getLocation() == Location::GAME) {
                    if (jsonObj.contains("params")) {
                        if (jsonObj["params"].contains("soldiers")) {
                            std::thread th(&GameMinorTowns::raiseArmy, this->gameMinorTowns, user, jsonObj["params"]["soldiers"]);
                            th.detach();
                            return MIDDLEWARE_STATUS::ST_OK;
                        }
                    }
                    user->messagePool.pushBackMessage("{\"err\": \"param problem\"}");
                    return MIDDLEWARE_STATUS::ST_ERROR;
            } else if (jsonObj["action"] == "disband_army" && user->getLocation() == Location::GAME) {
                    if (jsonObj.contains("params")) {
                        if (jsonObj["params"].contains("soldiers")) {
                            std::thread th(&GameMinorTowns::disbandArmy, this->gameMinorTowns, user, jsonObj["params"]["soldiers"]);
                            th.detach();
                            return MIDDLEWARE_STATUS::ST_OK;
                        }
                    }
                    user->messagePool.pushBackMessage("{\"err\": \"param problem\"}");
                    return MIDDLEWARE_STATUS::ST_ERROR;
            }
        } else {
                return MIDDLEWARE_STATUS::ST_ERROR;
        }
    }
    catch (Poco::Exception&) {
        return MIDDLEWARE_STATUS::ST_ERROR;
    }
    return MIDDLEWARE_STATUS::ST_OK;
}

MIDDLEWARE_STATUS Middleware::disconnect(std::shared_ptr<User> user)
{
    auto location = user->getLocation();
    if (location == Location::MENU) {
        std::thread th(&GameMinorTowns::unsubscribeUpdateLobby, this->gameMinorTowns, user);
        th.detach();
        return MIDDLEWARE_STATUS::ST_OK;
    }
    else if (location == Location::LOBBY) {
        std::thread th(&GameMinorTowns::leaveLobby, this->gameMinorTowns, user);
        th.detach();
        return MIDDLEWARE_STATUS::ST_OK;
    }
    else if (location == Location::GAME) {
        
        return MIDDLEWARE_STATUS::ST_OK;
    }
    return MIDDLEWARE_STATUS::ST_ERROR;
}

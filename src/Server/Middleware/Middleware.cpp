#include "Middleware.h"
#include "Poco\JSON\JSON.h"
#include "Poco\JSON\Parser.h"
#include "GameMinorTowns/GameMinorTowns.h"
#include <thread>

Middleware::Middleware(std::shared_ptr<GameMinorTowns> game)
{
    this->gameMinorTowns = game;
}

Middleware::~Middleware()
{
}

std::shared_ptr<User> Middleware::Authorization(std::string username, std::string address, unsigned short port)
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
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result;
        result = parser.parse(jsonMessage);
        Poco::JSON::Object::Ptr jsonObj = result.extract<Poco::JSON::Object::Ptr>();
        if (jsonObj->has("action")) {
            Poco::Dynamic::Var value = jsonObj->get("action");
            std::string actionTODO = value.toString();
            std::cout << "Value of action: " << actionTODO << std::endl;
            if (actionTODO == "create_game" && user->getLocation() == Location::MENU) {
                if (jsonObj->has("params")) {
                    Poco::JSON::Object::Ptr paramsObject = jsonObj->getObject("params");
                    if (paramsObject->has("max_users")) {
                        int maxUsers = paramsObject->getValue<int>("max_users");
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
            else if (actionTODO == "join_lobby" && user->getLocation() == Location::MENU) {
                if (jsonObj->has("params")) {
                    Poco::JSON::Object::Ptr paramsObject = jsonObj->getObject("params");
                    if (paramsObject->has("uuid")) {
                        std::string uuid = paramsObject->getValue<std::string>("uuid");
                        if (!uuid.empty()) {
                            std::thread th(&GameMinorTowns::joinLobby, this->gameMinorTowns, user, uuid);
                            th.detach();
                            return MIDDLEWARE_STATUS::ST_OK;
                        }
                    }
                }
                user->messagePool.pushBackMessage("{\"err\": \"param problem\"}");
                return MIDDLEWARE_STATUS::ST_ERROR;
            } 
            else if (actionTODO == "leave_Lobby" && user->getLocation() == Location::LOBBY) {
                std::thread th(&GameMinorTowns::leaveLobby, this->gameMinorTowns, user);
                th.detach();
                return MIDDLEWARE_STATUS::ST_OK;
            }
            else if (actionTODO == "subscribe_update_lobby" && user->getLocation() == Location::MENU) {
                std::thread th(&GameMinorTowns::subscribeUpdateLobby, this->gameMinorTowns, user);
                th.detach();
                return MIDDLEWARE_STATUS::ST_OK;
            }
            else if (actionTODO == "unsubscribe_update_lobby" && user->getLocation() == Location::MENU) {
                std::thread th(&GameMinorTowns::unsubscribeUpdateLobby, this->gameMinorTowns, user);
                th.detach();
                return MIDDLEWARE_STATUS::ST_OK;
            }
            else if (actionTODO == "build_buildings" && user->getLocation() == Location::GAME) {
                if (jsonObj->has("params")) {
                    Poco::JSON::Object::Ptr paramsObject = jsonObj->getObject("params");
                    if (paramsObject->has("building_id")) {
                        std::thread th(&GameMinorTowns::buildBuildings, this->gameMinorTowns, user, paramsObject->getValue<int>("building_id"));
                        th.detach();
                        return MIDDLEWARE_STATUS::ST_OK;
                    }
                }
                user->messagePool.pushBackMessage("{\"err\": \"param problem\"}");
                return MIDDLEWARE_STATUS::ST_ERROR;
            }
        }
        else {
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

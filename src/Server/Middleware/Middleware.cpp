#include "Middleware.h"
#include "GameMinorTowns/GameMinorTowns.h"
#include <thread>
#include "nlohmann\json.hpp"
#include <iostream>
#include <Poco\Exception.h>
#include "request_handler\params_validator.h"

Middleware::Middleware(std::shared_ptr<GameMinorTowns> game)
    : game_minor_towns(game)
{

}

Middleware::~Middleware()
{
}

std::shared_ptr<User> Middleware::authorization(std::string username, std::string address, unsigned short port)
{
    if (this->auth_validate.checkUsername(username)) {
        return nullptr;
    }
	std::shared_ptr<User> user;
	user = std::make_shared<User>(username, address + ":" + std::to_string(port));
	return user;
}

std::pair<MIDDLEWARE_STATUS, std::string> Middleware::action(RequestResult request_result, std::shared_ptr<User> user)
{
    if (request_result.operation == RequestOperation::CREATE_GAME && user->getLocation() == Location::MENU) {
        if (request_result.isParams()) {
            const Params::CreateGame *params = static_cast<const Params::CreateGame*>(request_result.getParams());
            if (ParamsValidator::validate(params) == nullptr) {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("failed validation"));
            }
            std::thread th(&GameMinorTowns::createLobby, this->game_minor_towns, user, params->max_users);
            th.detach();
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
        }
        else {
            return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("missing params"));
        }
    }
    else if (request_result.operation == RequestOperation::JOIN_LOBBY && user->getLocation() == Location::MENU) {
        if (request_result.isParams()) {
            const Params::JoinLobby* params = static_cast<const Params::JoinLobby*>(request_result.getParams());
            if (ParamsValidator::validate(params) == nullptr) {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("failed validation"));
            }
            std::thread th(&GameMinorTowns::joinLobby, this->game_minor_towns, user, params->uuid_lobby);
            th.detach();
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
        }
        else {
            return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("missing params"));
        }
    } else if (request_result.operation == RequestOperation::LEAVE_LOBBY && user->getLocation() == Location::LOBBY) {
            std::thread th(&GameMinorTowns::leaveLobby, this->game_minor_towns, user);
            th.detach();
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
    } else if (request_result.operation == RequestOperation::SUBSCRIBE_UPDATE && user->getLocation() == Location::MENU) {
            std::thread th(&GameMinorTowns::subscribeUpdateLobby, this->game_minor_towns, user);
            th.detach();
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
    } else if (request_result.operation == RequestOperation::UNSUBSCRIBE_UPDATE && user->getLocation() == Location::MENU) {
            std::thread th(&GameMinorTowns::unsubscribeUpdateLobby, this->game_minor_towns, user);
            th.detach();
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
    } else if (request_result.operation == RequestOperation::BUILD_BUILDINGS && user->getLocation() == Location::GAME) {
            if (request_result.isParams()) {
                const Params::BuildBuildings* params = static_cast<const Params::BuildBuildings*>(request_result.getParams());
                if (ParamsValidator::validate(params) == nullptr) {
                    return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("failed validation"));
                }
                std::thread th(&GameMinorTowns::buildBuildings, this->game_minor_towns, user, params->building_id);
                th.detach();
                return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
            }
            else {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("missing params"));
            }
    } else if (request_result.operation == RequestOperation::RAISE_ARMY && user->getLocation() == Location::GAME) {
            if (request_result.isParams()) {
                const Params::RaiseArmy* params = static_cast<const Params::RaiseArmy*>(request_result.getParams());
                if (ParamsValidator::validate(params) == nullptr) {
                    return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("failed validation"));
                }
                std::thread th(&GameMinorTowns::raiseArmy, this->game_minor_towns, user, params->soldiers);
                th.detach();
                return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
            }
            else {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("missing params"));
            }
    } else if (request_result.operation == RequestOperation::DISBAND_ARMY && user->getLocation() == Location::GAME) {
            if (request_result.isParams()) {
                const Params::DisbandArmy* params = static_cast<const Params::DisbandArmy*>(request_result.getParams());
                if (ParamsValidator::validate(params) == nullptr) {
                    return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("failed validation"));
                }
                std::thread th(&GameMinorTowns::disbandArmy, this->game_minor_towns, user, params->soldiers);
                th.detach();
                return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
            }
            else {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("missing params"));
            }
    } else {
        return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("unknown action"));
    }
}

MIDDLEWARE_STATUS Middleware::disconnect(std::shared_ptr<User> user)
{
    auto location = user->getLocation();
    if (location == Location::MENU) {
        std::thread th(&GameMinorTowns::unsubscribeUpdateLobby, this->game_minor_towns, user);
        th.detach();
        return MIDDLEWARE_STATUS::ST_OK;
    }
    else if (location == Location::LOBBY) {
        std::thread th(&GameMinorTowns::leaveLobby, this->game_minor_towns, user);
        th.detach();
        return MIDDLEWARE_STATUS::ST_OK;
    }
    else if (location == Location::GAME) {
        
        return MIDDLEWARE_STATUS::ST_OK;
    }
    return MIDDLEWARE_STATUS::ST_ERROR;
}

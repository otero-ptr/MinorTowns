#include "Middleware.h"
#include "GameMinorTowns/GameMinorTowns.h"
#include <thread>
#include "nlohmann\json.hpp"
#include <Poco\Exception.h>
#include "request_handler\params_validator.h"
#include "AuthorizatonValidate/AuthorizationValidate.h"
#include "future"

Middleware::Middleware(std::shared_ptr<GameMinorTowns> game)
    : game_minor_towns(game)
{

}

Middleware::~Middleware()
{
}

std::shared_ptr<User> Middleware::authorization(std::string username, std::string address, unsigned short port)
{
    if (AuthValidate::checkUsername(username)) {
        return nullptr;
    }
	std::shared_ptr<User> user;
	user = std::make_shared<User>(username, address + ":" + std::to_string(port));
	return user;
}

std::pair<MIDDLEWARE_STATUS, std::string> Middleware::action(RequestResult request_result, std::shared_ptr<User> user)
{
    if (request_result.getOperation() == RequestOperation::CREATE_LOBBY && user->getLocation() == Location::MENU) {
        if (request_result.isParams()) {
            const Params::CreateLobby *params = static_cast<const Params::CreateLobby*>(request_result.getParams());
            if (ParamsValidator::validate(params) == nullptr) {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("failed validation"));
            }
            auto result = std::async(&GameMinorTowns::createLobby, this->game_minor_towns, user, params->max_users);
            result.wait();
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
        }
        else {
            return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("missing params"));
        }
    }
    else if (request_result.getOperation() == RequestOperation::JOIN_LOBBY && user->getLocation() == Location::MENU) {
        if (request_result.isParams()) {
            const Params::JoinLobby* params = static_cast<const Params::JoinLobby*>(request_result.getParams());
            if (ParamsValidator::validate(params) == nullptr) {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("failed validation"));
            }
            auto result = std::async(&GameMinorTowns::joinLobby, this->game_minor_towns, user, params->uuid_lobby);
            result.wait();
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
        }
        else {
            return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("missing params"));
        }
    } else if (request_result.getOperation() == RequestOperation::LEAVE_LOBBY && user->getLocation() == Location::LOBBY) {
            auto result = std::async(&GameMinorTowns::leaveLobby, this->game_minor_towns, user);
            result.wait();
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
    } else if (request_result.getOperation() == RequestOperation::SUBSCRIBE_UPDATE && user->getLocation() == Location::MENU) {
            auto result = std::async(&GameMinorTowns::subscribeUpdateLobby, this->game_minor_towns, user);
            result.wait();
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
    } else if (request_result.getOperation() == RequestOperation::UNSUBSCRIBE_UPDATE && user->getLocation() == Location::MENU) {
            auto result = std::async(&GameMinorTowns::unsubscribeUpdateLobby, this->game_minor_towns, user);
            result.wait();
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
    } else if (request_result.getOperation() == RequestOperation::BUILD_BUILDINGS && user->getLocation() == Location::GAME) {
            if (request_result.isParams()) {
                const Params::BuildBuildings* params = static_cast<const Params::BuildBuildings*>(request_result.getParams());
                if (ParamsValidator::validate(params) == nullptr) {
                    return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("failed validation"));
                }
                auto result = std::async(&GameMinorTowns::buildBuildings, this->game_minor_towns, user, params->building_id);
                result.wait();
                return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
            }
            else {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("missing params"));
            }
    } else if (request_result.getOperation() == RequestOperation::RAISE_ARMY && user->getLocation() == Location::GAME) {
            if (request_result.isParams()) {
                const Params::RaiseArmy* params = static_cast<const Params::RaiseArmy*>(request_result.getParams());
                if (ParamsValidator::validate(params) == nullptr) {
                    return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("failed validation"));
                }
                auto result = std::async(&GameMinorTowns::raiseArmy, this->game_minor_towns, user, params->soldiers);
                result.wait();
                return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
            }
            else {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("missing params"));
            }
    } else if (request_result.getOperation() == RequestOperation::DISBAND_ARMY && user->getLocation() == Location::GAME) {
            if (request_result.isParams()) {
                const Params::DisbandArmy* params = static_cast<const Params::DisbandArmy*>(request_result.getParams());
                if (ParamsValidator::validate(params) == nullptr) {
                    return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("failed validation"));
                }
                auto result = std::async(&GameMinorTowns::disbandArmy, this->game_minor_towns, user, params->soldiers);
                result.wait();
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

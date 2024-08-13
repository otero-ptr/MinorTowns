#include "Middleware.h"
#include "GameMinorTowns/GameMinorTowns.h"
#include <thread>
#include "nlohmann\json.hpp"
#include <Poco\Exception.h>
#include "RequestHandler\ParamsValidator.h"
#include "AuthorizatonValidate/AuthorizationValidate.h"
#include "future"

Middleware::Middleware(std::shared_ptr<GameMinorTowns> game)
    : game_minor_towns(game) {

}

Middleware::~Middleware() {

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
            auto params = request_result.getParams().lock();
            auto lobby_params = std::static_pointer_cast<Params::CreateLobby>(params);
            if (ParamsValidator::validate(lobby_params)) {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("failed validation"));
            }
            auto result = std::async(&GameMinorTowns::createLobby, game_minor_towns, std::ref(user), lobby_params->max_users);
            result.wait();
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
        }
        else {
            return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("missing params"));
        }
    }
    else if (request_result.getOperation() == RequestOperation::JOIN_LOBBY && user->getLocation() == Location::MENU) {
        if (request_result.isParams()) {
            auto params = request_result.getParams().lock();
            auto join_lobby_params = std::static_pointer_cast<Params::JoinLobby>(params);
            if (ParamsValidator::validate(join_lobby_params)) {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("failed validation"));
            }
            auto result = std::async(&GameMinorTowns::joinLobby, game_minor_towns, std::ref(user), join_lobby_params->uuid_lobby);
            result.wait();
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
        }
        else {
            return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("missing params"));
        }
    } else if (request_result.getOperation() == RequestOperation::LEAVE_LOBBY && user->getLocation() == Location::LOBBY) {
            auto result = std::async(&GameMinorTowns::leaveLobby, game_minor_towns, std::ref(user));
            result.wait();
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
    } else if (request_result.getOperation() == RequestOperation::SUBSCRIBE_UPDATE && user->getLocation() == Location::MENU) {
            auto result = std::async(&GameMinorTowns::subscribeUpdateLobby, game_minor_towns, std::ref(user));
            result.wait();
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
    } else if (request_result.getOperation() == RequestOperation::UNSUBSCRIBE_UPDATE && user->getLocation() == Location::MENU) {
            auto result = std::async(&GameMinorTowns::unsubscribeUpdateLobby, game_minor_towns, std::ref(user));
            result.wait();
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
    } else if (request_result.getOperation() == RequestOperation::BUILD_BUILDINGS && user->getLocation() == Location::GAME) {
            if (request_result.isParams()) {
                auto params = request_result.getParams().lock();
                auto buildings_params = std::static_pointer_cast<Params::BuildBuildings>(params);
                if (ParamsValidator::validate(buildings_params)) {
                    return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("failed validation"));
                }
                auto result = std::async(&GameMinorTowns::buildBuildings, game_minor_towns, std::ref(user), buildings_params->building_id);
                result.wait();
                return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
            }
            else {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("missing params"));
            }
    } else if (request_result.getOperation() == RequestOperation::RAISE_ARMY && user->getLocation() == Location::GAME) {
            if (request_result.isParams()) {
                auto params = request_result.getParams().lock();
                auto raise_params = std::static_pointer_cast<Params::RaiseArmy>(params);
                if (ParamsValidator::validate(raise_params)) {
                    return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("failed validation"));
                }
                auto result = std::async(&GameMinorTowns::raiseArmy, game_minor_towns, std::ref(user), raise_params->soldiers);
                result.wait();
                return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
            }
            else {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("missing params"));
            }
    } else if (request_result.getOperation() == RequestOperation::DISBAND_ARMY && user->getLocation() == Location::GAME) {
            if (request_result.isParams()) {
                auto params = request_result.getParams().lock();
                auto disband_params = std::static_pointer_cast<Params::DisbandArmy>(params);
                if (ParamsValidator::validate(disband_params)) {
                    return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("failed validation"));
                }
                auto result = std::async(&GameMinorTowns::disbandArmy, game_minor_towns, std::ref(user), disband_params->soldiers);
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
    if (user->getLocation() == Location::MENU) {
        auto result = std::async(&GameMinorTowns::unsubscribeUpdateLobby, game_minor_towns, std::ref(user));
        result.wait();
        return MIDDLEWARE_STATUS::ST_OK;
    }
    else if (user->getLocation() == Location::LOBBY) {
        auto result = std::async(&GameMinorTowns::leaveLobby, game_minor_towns, std::ref(user));
        result.wait();
        return MIDDLEWARE_STATUS::ST_OK;
    }
    else if (user->getLocation() == Location::GAME) {
        
        return MIDDLEWARE_STATUS::ST_OK;
    }
    return MIDDLEWARE_STATUS::ST_ERROR;
}

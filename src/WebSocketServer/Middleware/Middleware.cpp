#include "Middleware.h"
#include <thread>
#include "IManagerController.h"
#include "AuthorizationValidate/AuthorizationValidate.h"
#include "User.h"
#include "RequestResult.h"
#include <future>

Middleware::Middleware(std::shared_ptr<IManagerController> manager_controller)
    : manager_controller(manager_controller) {

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
            auto result = std::async(&IManagerController::createLobby, manager_controller, std::ref(user), lobby_params->max_users);
            result.wait();
            auto value = result.get();
            if (value.has_value()) {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string(value.value()));
            }
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
            auto result = std::async(&IManagerController::joinLobby, manager_controller, std::ref(user), join_lobby_params->uuid_lobby);
            result.wait();
            auto value = result.get();
            if (value.has_value()) {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string(value.value()));
            }
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
        }
        else {
            return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("missing params"));
        }
    } else if (request_result.getOperation() == RequestOperation::LEAVE_LOBBY && user->getLocation() == Location::LOBBY) {
            auto result = std::async(&IManagerController::leaveLobby, manager_controller, std::ref(user));
            result.wait();
            auto value = result.get();
            if (value.has_value()) {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string(value.value()));
            }
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
    } else if (request_result.getOperation() == RequestOperation::SUBSCRIBE_UPDATE && user->getLocation() == Location::MENU) {
            auto result = std::async(&IManagerController::subscribeUpdateLobby, manager_controller, std::ref(user));
            result.wait();
            auto value = result.get();
            if (value.has_value()) {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string(value.value()));
            }
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
    } else if (request_result.getOperation() == RequestOperation::UNSUBSCRIBE_UPDATE && user->getLocation() == Location::MENU) {
            auto result = std::async(&IManagerController::unsubscribeUpdateLobby, manager_controller, std::ref(user));
            result.wait();
            auto value = result.get();
            if (value.has_value()) {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string(value.value()));
            }
            return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
    } else if (request_result.getOperation() == RequestOperation::BUILD_BUILDINGS && user->getLocation() == Location::GAME) {
            if (request_result.isParams()) {
                auto params = request_result.getParams().lock();
                auto buildings_params = std::static_pointer_cast<Params::BuildBuildings>(params);
                auto result = std::async(&IManagerController::buildBuildings, manager_controller, std::ref(user), buildings_params->building_id);
                result.wait();
                auto value = result.get();
                if (value.has_value()) {
                    return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string(value.value()));
                }
                return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
            }
            else {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("missing params"));
            }
    } else if (request_result.getOperation() == RequestOperation::RAISE_ARMY && user->getLocation() == Location::GAME) {
            if (request_result.isParams()) {
                auto params = request_result.getParams().lock();
                auto raise_params = std::static_pointer_cast<Params::RaiseArmy>(params);
                auto result = std::async(&IManagerController::raiseArmy, manager_controller, std::ref(user), raise_params->soldiers);
                result.wait();
                auto value = result.get();
                if (value.has_value()) {
                    return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string(value.value()));
                }
                return std::make_pair(MIDDLEWARE_STATUS::ST_OK, std::string());
            }
            else {
                return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string("missing params"));
            }
    } else if (request_result.getOperation() == RequestOperation::DISBAND_ARMY && user->getLocation() == Location::GAME) {
            if (request_result.isParams()) {
                auto params = request_result.getParams().lock();
                auto disband_params = std::static_pointer_cast<Params::DisbandArmy>(params);
                auto result = std::async(&IManagerController::disbandArmy, manager_controller, std::ref(user), disband_params->soldiers);
                result.wait();
                auto value = result.get();
                if (value.has_value()) {
                    return std::make_pair(MIDDLEWARE_STATUS::ST_ERROR, std::string(value.value()));
                }
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
        auto result = std::async(&IManagerController::unsubscribeUpdateLobby, manager_controller, std::ref(user));
        result.wait();
        auto value = result.get();
        if (value.has_value()) {
            return MIDDLEWARE_STATUS::ST_ERROR;
        }
        return MIDDLEWARE_STATUS::ST_OK;
    }
    else if (user->getLocation() == Location::LOBBY) {
        auto result = std::async(&IManagerController::leaveLobby, manager_controller, std::ref(user));
        result.wait();
        auto value = result.get();
        if (value.has_value()) {
            return MIDDLEWARE_STATUS::ST_ERROR;
        }
        return MIDDLEWARE_STATUS::ST_OK;
    }
    else if (user->getLocation() == Location::GAME) {
        auto result = std::async(&IManagerController::leftGame, manager_controller, std::ref(user));
        result.wait();
        auto value = result.get();
        if (value.has_value()) {
            return MIDDLEWARE_STATUS::ST_ERROR;
        }
        return MIDDLEWARE_STATUS::ST_OK;
    }
    return MIDDLEWARE_STATUS::ST_ERROR;
}

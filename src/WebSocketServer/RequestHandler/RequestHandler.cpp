#include "RequestHandler.h"
#include "ParamsValidator.h"

RequestHandler::RequestHandler()
{
}

std::variant<RequestResult, RequestError> RequestHandler::Handler(std::string request)
{
    try {
        nlohmann::json request_json = nlohmann::json::parse(request);
        if (request_json.contains("action")) {
            if (request_json["action"]== "create_lobby") {
                return CreateLobby(request_json);
            }
            else if (request_json["action"] == "join_lobby") {
                return JoinLobby(request_json);
            }
            else if (request_json["action"] == "leave_lobby") {
                return LeaveLobby(request_json);
            }
            else if (request_json["action"] == "subscribe_update_lobby") {
                return SubscribeUpdateLobby(request_json);
            }
            else if (request_json["action"] == "unsubscribe_update_lobby") {
                return UnsubscribeUpdateLobby(request_json);
            }
            else if (request_json["action"] == "build_buildings") {
                return BuildBuildings(request_json);
            }
            else if (request_json["action"] == "raise_army") {
                return RaiseArmy(request_json);
            }
            else if (request_json["action"] == "disband_army") {
                return DisbandArmy(request_json);
            } else {
                return RequestError("Action doesn't exist");
            }
        }
        else {
            return RequestError("Action not specified");
        }
    }
    catch (std::exception& ex) {
        
        return RequestError(ex.what());
    }
    catch (...) {
        return RequestError("unknown handler error");
    }
}

std::variant<RequestResult, RequestError> RequestHandler::CreateLobby(nlohmann::json request_json)
{
    if (request_json.contains("params")) {
        std::shared_ptr params = std::make_shared<Params::CreateLobby>();
        if (request_json["params"].contains("max_users")) {
            params->max_users = request_json["params"]["max_users"].get<uint32_t>();
        } else {
            return RequestError("field max_users missing");
        }
        if (!ParamsValidator::validate(params)) {
            return RequestError("failed validation");
        }
        return RequestResult(RequestOperation::CREATE_LOBBY, std::move(params));
    } else {
        return RequestError("missing parameters");
    }
}

std::variant<RequestResult, RequestError> RequestHandler::JoinLobby(nlohmann::json request_json)
{
    if (request_json.contains("params")) {
        std::shared_ptr params = std::make_shared<Params::JoinLobby>();
        if (request_json["params"].contains("uuid")) {
            params->uuid_lobby = request_json["params"]["uuid"].get<std::string>();
        } else {
            return RequestError("field uuid missing");
        }
        if (!ParamsValidator::validate(params)) {
            return RequestError("failed validation");
        }
        return RequestResult(RequestOperation::JOIN_LOBBY, std::move(params));
    } else {
        return RequestError("missing parameters");
    }
}

std::variant<RequestResult, RequestError> RequestHandler::LeaveLobby(nlohmann::json request_json)
{
    return RequestResult(RequestOperation::LEAVE_LOBBY);
}

std::variant<RequestResult, RequestError> RequestHandler::SubscribeUpdateLobby(nlohmann::json request_json)
{
    return RequestResult(RequestOperation::SUBSCRIBE_UPDATE);
}

std::variant<RequestResult, RequestError> RequestHandler::UnsubscribeUpdateLobby(nlohmann::json request_json)
{
    return RequestResult(RequestOperation::UNSUBSCRIBE_UPDATE);
}

std::variant<RequestResult, RequestError> RequestHandler::BuildBuildings(nlohmann::json request_json)
{
    if (request_json.contains("params")) {
        std::shared_ptr params = std::make_shared<Params::BuildBuildings>();
        if (request_json["params"].contains("building_id")) {
            params->building_id = request_json["params"]["building_id"].get<uint32_t>();
        }
        if (!ParamsValidator::validate(params)) {
            return RequestError("failed validation");
        }
        return RequestResult(RequestOperation::BUILD_BUILDINGS, std::move(params));
    } else {
        return RequestError("missing parameters");
    }
}

std::variant<RequestResult, RequestError> RequestHandler::RaiseArmy(nlohmann::json request_json)
{
    if (request_json.contains("params")) {
        std::shared_ptr params = std::make_shared<Params::RaiseArmy>();
        if (request_json["params"].contains("soldiers")) {
            params->soldiers = request_json["params"]["soldiers"].get<uint32_t>();
        } else {
            return RequestError("field soldiers missing");
        }
        if (!ParamsValidator::validate(params)) {
            return RequestError("failed validation");
        }
        return RequestResult(RequestOperation::RAISE_ARMY,std::move(params));
    } else {
        return RequestError("missing parameters");
    }
}

std::variant<RequestResult, RequestError> RequestHandler::DisbandArmy(nlohmann::json request_json)
{
    if (request_json.contains("params")) {
        std::shared_ptr params = std::make_shared<Params::DisbandArmy>();
        if (request_json["params"].contains("soldiers")) {
            params->soldiers = request_json["params"]["soldiers"].get<uint32_t>();
        } else {
            return RequestError("field soldiers missing");
        }
        if (!ParamsValidator::validate(params)) {
            return RequestError("failed validation");
        }
        return RequestResult(RequestOperation::DISBAND_ARMY, std::move(params));
    } else {
        return RequestError("missing parameters");
    }
}

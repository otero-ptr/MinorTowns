#include "request_handler.h"
#include <iostream>

RequestHandler::RequestHandler()
{
}

RequestHandler::~RequestHandler()
{
}

std::optional<RequestResult> RequestHandler::Handler(std::string request)
{
    try {
        nlohmann::json request_json = nlohmann::json::parse(request);
        if (request_json.contains("action")) {
            std::cout << "Value of action: " << request_json["action"] << std::endl;
            if (request_json["action"].get<std::string>() == "create_game") {
                return this->CreateGame(request_json);
            }
            else if (request_json["action"] == "join_lobby") {
                return this->JoinLobby(request_json);
            }
            else if (request_json["action"] == "leave_lobby") {
                return this->LeaveLobby(request_json);
            }
            else if (request_json["action"] == "subscribe_update_lobby") {
                return this->SubscribeUpdateLobby(request_json);
            }
            else if (request_json["action"] == "unsubscribe_update_lobby") {
                return this->UnsubscribeUpdateLobby(request_json);
            }
            else if (request_json["action"] == "build_buildings") {
                return this->BuildBuildings(request_json);
            }
            else if (request_json["action"] == "raise_army") {
                return this->RaiseArmy(request_json);
            }
            else if (request_json["action"] == "disband_army") {
                return this->DisbandArmy(request_json);
            } else {
                return std::nullopt;
            }
        }
        else {
            return std::nullopt;
        }
    }
    catch (std::exception& ex) {
        RequestResult request_result;
        request_result.setErrorInfo(new ErrorInfo(ex.what()));
        return request_result;
    }
    catch (...) {
        RequestResult request_result;
        request_result.setErrorInfo(new ErrorInfo("unknown error"));
        return request_result;
    }
}

RequestResult RequestHandler::CreateGame(nlohmann::json request_json)
{
    RequestResult request_result;
    request_result.operation = RequestOperation::CREATE_GAME;
    if (request_json.contains("params")) {
        std::unique_ptr params = std::make_unique<Params::CreateGame>();
        if (request_json["params"].contains("max_users")) {
            params->max_users = request_json["params"]["max_users"].get<uint32_t>();
        } else {
            request_result.setErrorInfo(new ErrorInfo("field max_users missing"));
            return request_result;
        }
        request_result.setParams(params.release());
    } else {
        request_result.setErrorInfo(new ErrorInfo("missing parameters"));
    }
    return request_result;
}

RequestResult RequestHandler::JoinLobby(nlohmann::json request_json)
{
    RequestResult request_result;
    request_result.operation = RequestOperation::JOIN_LOBBY;
    if (request_json.contains("params")) {
        std::unique_ptr params = std::make_unique<Params::JoinLobby>();
        if (request_json["params"].contains("uuid")) {
            params->uuid_lobby = request_json["params"]["uuid"].get<std::string>();
        } else {
            request_result.setErrorInfo(new ErrorInfo("field uuid missing"));
            return request_result;
        }
        request_result.setParams(params.release());
    } else {
        request_result.setErrorInfo(new ErrorInfo("missing parameters"));
    }
    return request_result;
}

RequestResult RequestHandler::LeaveLobby(nlohmann::json request_json)
{
    RequestResult request_result;
    request_result.operation = RequestOperation::LEAVE_LOBBY;
    return request_result;
}

RequestResult RequestHandler::SubscribeUpdateLobby(nlohmann::json request_json)
{
    RequestResult request_result;
    request_result.operation = RequestOperation::SUBSCRIBE_UPDATE;
    return request_result;
}

RequestResult RequestHandler::UnsubscribeUpdateLobby(nlohmann::json request_json)
{
    RequestResult request_result;
    request_result.operation = RequestOperation::UNSUBSCRIBE_UPDATE;
    return request_result;
}

RequestResult RequestHandler::BuildBuildings(nlohmann::json request_json)
{
    RequestResult request_result;
    request_result.operation = RequestOperation::BUILD_BUILDINGS;
    if (request_json.contains("params")) {
        std::unique_ptr params = std::make_unique<Params::BuildBuildings>();
        if (request_json["params"].contains("building_id")) {
            params->building_id = request_json["params"]["building_id"].get<uint32_t>();
        }
        request_result.setParams(params.release());
    } else {
        request_result.setErrorInfo(new ErrorInfo("missing parameters"));
    }
    return request_result;
}

RequestResult RequestHandler::RaiseArmy(nlohmann::json request_json)
{
    RequestResult request_result;
    request_result.operation = RequestOperation::RAISE_ARMY;
    if (request_json.contains("params")) {
        std::unique_ptr params = std::make_unique<Params::RaiseArmy>();
        if (request_json["params"].contains("soldiers")) {
            params->soldiers = request_json["params"]["soldiers"].get<uint32_t>();
        } else {
            request_result.setErrorInfo(new ErrorInfo("field soldiers missing"));
            return request_result;
        }
        request_result.setParams(params.release());
    } else {
        request_result.setErrorInfo(new ErrorInfo("missing parameters"));
    }
    return request_result;
}

RequestResult RequestHandler::DisbandArmy(nlohmann::json request_json)
{
    RequestResult request_result;
    request_result.operation = RequestOperation::DISBAND_ARMY;
    if (request_json.contains("params")) {
        std::unique_ptr params = std::make_unique<Params::DisbandArmy>();
        if (request_json["params"].contains("soldiers")) {
            params->soldiers = request_json["params"]["soldiers"].get<uint32_t>();
        } else {
            request_result.setErrorInfo(new ErrorInfo("field soldiers missing"));
            return request_result;
        }
        request_result.setParams(params.release());
    } else {
        request_result.setErrorInfo(new ErrorInfo("missing parameters"));
    }
    return request_result;
}

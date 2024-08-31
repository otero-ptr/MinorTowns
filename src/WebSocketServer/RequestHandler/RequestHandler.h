#pragma once
#include <string>
#include <variant>
#include "RequestResult.h"
#include "RequestError.h"
#include "nlohmann\json.hpp"

class RequestHandler {
public:
	static std::variant<RequestResult, RequestError> Handler(std::string request);
private:
	RequestHandler();
	static std::variant<RequestResult, RequestError> CreateLobby(nlohmann::json request_json);
	static std::variant<RequestResult, RequestError> JoinLobby(nlohmann::json request_json);
	static std::variant<RequestResult, RequestError> LeaveLobby(nlohmann::json request_json);
	static std::variant<RequestResult, RequestError> SubscribeUpdateLobby(nlohmann::json request_json);
	static std::variant<RequestResult, RequestError> UnsubscribeUpdateLobby(nlohmann::json request_json);
	static std::variant<RequestResult, RequestError> BuildBuildings(nlohmann::json request_json);
	static std::variant<RequestResult, RequestError> RaiseArmy(nlohmann::json request_json);
	static std::variant<RequestResult, RequestError> DisbandArmy(nlohmann::json request_json);
	static std::variant<RequestResult, RequestError> MoveArmy(nlohmann::json request_json);
};
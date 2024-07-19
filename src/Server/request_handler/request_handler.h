#pragma once
#include <string>
#include <optional>
#include "request_result.h"
#include "nlohmann\json.hpp"

class RequestHandler {
public:
	RequestHandler();
	~RequestHandler();
	std::optional<RequestResult> Handler(std::string request);
private:
	RequestResult CreateGame(nlohmann::json request_json);
	RequestResult JoinLobby(nlohmann::json request_json);
	RequestResult LeaveLobby(nlohmann::json request_json);
	RequestResult SubscribeUpdateLobby(nlohmann::json request_json);
	RequestResult UnsubscribeUpdateLobby(nlohmann::json request_json);
	RequestResult BuildBuildings(nlohmann::json request_json);
	RequestResult RaiseArmy(nlohmann::json request_json);
	RequestResult DisbandArmy(nlohmann::json request_json);
};
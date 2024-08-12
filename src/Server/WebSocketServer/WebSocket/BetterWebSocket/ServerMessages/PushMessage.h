#pragma once
#include "HttpStatusCodes.h"
#include <string>

struct PushMessage {
	PushMessage() = delete;
	PushMessage(std::string body) : body(body) { }
	std::string body;
};
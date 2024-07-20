#pragma once
#include "http_status_codes.h"
#include <string>

struct PushMessage {
	PushMessage() = delete;
	PushMessage(std::string body) : body(body) { }
	std::string body;
};
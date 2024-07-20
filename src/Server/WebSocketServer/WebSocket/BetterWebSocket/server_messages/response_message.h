#pragma once
#include "http_status_codes.h"
#include <string>

struct ResponseMessage {
	ResponseMessage() = delete;
	ResponseMessage(Code code, std::string body) : code(code), body(body) { }
	ResponseMessage(Code code) : code(code) { }
	Code code;
	std::string body;
};
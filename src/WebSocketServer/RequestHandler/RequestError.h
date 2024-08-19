#pragma once
#include <string>

struct RequestError {
	RequestError(std::string err)
		: err(err) {

	}
	std::string err;
};
#pragma once
#include <string>

struct ErrorInfo {
	ErrorInfo(std::string err_info)
		: err_info(err_info) {

	}
	std::string err_info;
};
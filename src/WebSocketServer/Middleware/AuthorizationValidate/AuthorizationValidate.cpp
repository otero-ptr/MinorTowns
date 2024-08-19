#include "AuthorizationValidate.h"
#include <regex>

bool AuthValidate::checkUsername(const std::string &username)
{
	if (username.size() < 4 || username.size() > 14) {
		return true;
	}
	if (!std::regex_match(username, std::regex("^[a-zA-Z0-9_]+$"))) {
		return true;
	}
	return false;
}
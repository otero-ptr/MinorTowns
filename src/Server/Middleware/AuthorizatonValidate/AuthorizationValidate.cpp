#include "AuthorizationValidate.h"
#include <regex>

bool AuthorizatonValidate::checkUsername(const std::string &username)
{
	if (username.size() < 5 || username.size() > 22) {
		return true;
	}
	if (!std::regex_match(username, std::regex("^[a-zA-Z0-9_]+$"))) {
		return true;
	}
	return false;
}
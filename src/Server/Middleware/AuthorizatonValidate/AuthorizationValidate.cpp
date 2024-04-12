#include "AuthorizationValidate.h"
#include <regex>

bool AuthorizatonValidate::checkUsername(const std::string &username)
{
	if (username.size() < 5 || username.size() > 22) {
		return true;
	}
	std::regex pattern("^[a-zA-Z0-9_ ]+$");
	if (!std::regex_match(username, pattern)) {
		return true;
	}
	return false;
}

bool AuthorizatonValidate::checkAddress(const std::string &address)
{
	std::regex pattern("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
	if (!std::regex_match(address, pattern)) {
		return true;
	}
	return false;
}

bool AuthorizatonValidate::checkPort(const unsigned short &port)
{
	if (!(port >= 0 && port <= std::numeric_limits<unsigned short>::max())) {
		return true;
	}
	return false;
}

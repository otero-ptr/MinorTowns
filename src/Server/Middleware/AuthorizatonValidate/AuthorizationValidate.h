#pragma once
#include <string>

class AuthorizatonValidate {
public:
	bool checkUsername(const std::string &username);
	bool checkAddress(const std::string &address);
	bool checkPort(const unsigned short &port);
};
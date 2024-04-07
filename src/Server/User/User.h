#pragma once
#include <string>

class User {
public:
	User() = delete;
	User(std::string username, std::string address);
	~User();
	bool operator==(const User& other) {
		return this->username == other.username && this->address == other.address;
	}
private:
	std::string username;
	std::string address;
};
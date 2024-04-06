#pragma once
#include <string>

class User {
public:
	User();
	~User();
	bool operator==(const User& other) {
		return this->unique == other.unique;
	}
private:
	std::string unique;
	std::string address;
};
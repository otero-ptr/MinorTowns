#pragma once
#include <string>

enum Location {
	MENU,
	LOBBY,
	GAME
};

class User {
public:
	User() = delete;
	User(std::string username, std::string address);
	~User();
	bool operator==(const User& other) {
		return this->username == other.username && this->address == other.address;
	}
	void setLocation(Location l);
	Location getLocation();
private:
	Location userLocation;
	std::string username;
	std::string address;
};
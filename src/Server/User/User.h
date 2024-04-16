#pragma once
#include <string>
#include "MessagePool\MessagePool.h"

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
	void setLocation(Location l, std::string uuid);
	Location getLocation();
	const std::string getUUIDLocation();
	std::string getUsername();
	MessagePool messagePool;
private:
	Location userLocation;
	std::string uuidLocation;
	std::string username;
	std::string address;
};
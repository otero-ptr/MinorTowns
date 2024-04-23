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
		return this->username == other.username && this->address == other.address && this->uuid == other.uuid;
	}
	void setLocation(Location l, std::string uuid);
	Location getLocation();
	const std::string getUUIDLocation();
	const std::string getUUID();
	std::string getUsername();
	MessagePool messagePool;
private:
	void generateUUID();
	Location userLocation;
	std::string uuidLocation;
	std::string uuid;
	std::string username;
	std::string address;
};
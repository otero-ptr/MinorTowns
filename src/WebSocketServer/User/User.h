#pragma once
#include <string>
#include "MessagePool\MessagePool.h"

enum class Location {
	MENU,
	LOBBY,
	GAME
};

class User {
public:
	User() = delete;
	User(std::string username, std::string address);
	~User();
	bool operator==(const User& other);
	void setLocation(Location l, std::string uuid);
	Location getLocation() const;
	const std::string getUUIDLocation() const;
	const std::string getUUID() const;
	std::string getUsername() const;
	MessagePool message_pool;
private:
	Location user_location;
	std::string uuid_location;
	std::string uuid;
	std::string username;
	std::string address;
};
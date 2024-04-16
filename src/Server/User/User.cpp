#include "User.h"

User::User(std::string username, std::string address)
{
	this->userLocation = MENU;
	this->username = username;
	this->address = address;
}

User::~User()
{
}

void User::setLocation(Location l, std::string uuid)
{
	this->userLocation = l;
	this->uuidLocation = uuid;
}

Location User::getLocation()
{
	return this->userLocation;
}

const std::string User::getUUIDLocation()
{
	return this->uuidLocation;
}

std::string User::getUsername()
{
	return this->username;
}

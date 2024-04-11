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

void User::setLocation(Location l)
{
	this->userLocation = l;
}

Location User::getLocation()
{
	return this->userLocation;
}

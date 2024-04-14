#include "User.h"

User::User(std::string username, std::string address)
{
	this->userLocation = MENU;
	this->username = username;
	this->address = address;
	this->userInfo = "{ }";
	this->updateInfo = false;
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

void User::setUserInfo(std::string jsonInfo)
{
	this->userInfo = jsonInfo;
	this->updateInfo = true;
}

bool User::isInfoUpdate()
{
	return this->updateInfo;
}

std::string User::getUserInfo()
{
	this->updateInfo = false;
	return this->userInfo;
}

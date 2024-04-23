#include "User.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/UUID.h"
User::User(std::string username, std::string address)
{
	this->generateUUID();
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

const std::string User::getUUID()
{
	return this->uuid;
}

std::string User::getUsername()
{
	return this->username;
}

void User::generateUUID()
{
	Poco::UUIDGenerator generator;
	Poco::UUID uuid = generator.create();
	this->uuid = uuid.toString();
}

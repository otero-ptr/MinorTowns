#include "User.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/UUID.h"
#include "util/gen_uuid.h"

User::User(std::string username, std::string address) 
	: uuid(GenerateUUID()), user_location(MENU),
	username(username), address(address)
{

}

User::~User()
{
}

void User::setLocation(Location l, std::string uuid)
{
	this->user_location = l;
	this->uuid_location = uuid;
}

Location User::getLocation()
{
	return this->user_location;
}

const std::string User::getUUIDLocation()
{
	return this->uuid_location;
}

const std::string User::getUUID()
{
	return this->uuid;
}

std::string User::getUsername()
{
	return this->username;
}

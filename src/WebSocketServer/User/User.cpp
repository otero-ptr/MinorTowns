#include "User.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/UUID.h"
#include "util/gen_uuid.h"

User::User(std::string username, std::string address) 
	: uuid(GenerateUUID()), user_location(Location::MENU),
	username(username), address(address)
{

}

User::~User()
{
}

bool User::operator==(const User& other)
{
	return username == other.username && address == other.address && uuid == other.uuid;
}

void User::setLocation(Location l, std::string uuid)
{
	user_location = l;
	uuid_location = uuid;
}

Location User::getLocation() const
{
	return user_location;
}

const std::string User::getUUIDLocation() const
{
	return uuid_location;
}

const std::string User::getUUID() const
{
	return uuid;
}

std::string User::getUsername() const
{
	return username;
}

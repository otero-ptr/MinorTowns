#include "Lobby.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/UUID.h"
#include "User/User.h"

Lobby::Lobby(int maxUser)
{
	this->maxUser = maxUser;
	this->users.reserve(maxUser);
	Poco::UUIDGenerator generator;
	Poco::UUID uuid = generator.create();
	this->unique = uuid.toString();
}

Lobby::~Lobby()
{
}

void Lobby::join(std::shared_ptr<User> user)
{
	if (this->users.size() < this->maxUser) {
		auto it = std::find(this->users.begin(), this->users.end(), user);
		if (it == this->users.end()) {
			this->users.push_back(user);
		}
	}
}

void Lobby::leave(std::shared_ptr<User> user)
{
	auto it = std::find(this->users.begin(), this->users.end(), user);
	if (it != this->users.end()) {
		this->users.erase(it);
	}
}

bool Lobby::isFull()
{
	return this->users.size() == this->maxUser;
}

std::vector<std::shared_ptr<User>>& Lobby::getUsers()
{
	return this->users;
}

const std::string Lobby::getUnique()
{
	return this->unique;
}

const int Lobby::getNowUsers()
{
	return this->users.size();
}

const int Lobby::getMaxUsers()
{
	return this->maxUser;
}
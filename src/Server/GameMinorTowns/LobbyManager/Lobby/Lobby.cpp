#include "Lobby.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/UUID.h"
#include "User/User.h"
#include "nlohmann\json.hpp"

Lobby::Lobby(int maxUser)
{
	this->maxUser = maxUser;
	this->users.reserve(maxUser);
	this->createUUID();
	this->createLobbyData();
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
		this->createLobbyData();
		this->notifyAllUsers();
	}
}

void Lobby::leave(std::shared_ptr<User> user)
{
	auto it = std::find(this->users.begin(), this->users.end(), user);
	if (it != this->users.end()) {
		this->users.erase(it);
	}
	this->createLobbyData();
	this->notifyAllUsers();
}

bool Lobby::isFull()
{
	return this->users.size() == this->maxUser;
}

bool Lobby::isEmpty()
{
	return this->users.size() == 0;
}

std::vector<std::shared_ptr<User>> Lobby::getUsers()
{
	return this->users;
}

const std::string Lobby::getUUID()
{
	return this->uuid;
}

const int Lobby::getNowUsers()
{
	return this->users.size();
}

const int Lobby::getMaxUsers()
{
	return this->maxUser;
}

std::string Lobby::getLobbyData()
{
	return this->lobbyData;
}

void Lobby::createLobbyData()
{
	nlohmann::json objJson;
	objJson["uuid_lobby"] = this->uuid;
	objJson["users_now"] = this->users.size();
	objJson["users_max"] = this->maxUser;
	this->lobbyData = objJson.dump();
}

void Lobby::createUUID()
{
	Poco::UUIDGenerator generator;
	Poco::UUID uuid = generator.create();
	this->uuid = uuid.toString();
}

void Lobby::notifyAllUsers()
{
	for (auto& user : this->users) {
		user->messagePool.pushBackMessage(this->lobbyData);
	}
}

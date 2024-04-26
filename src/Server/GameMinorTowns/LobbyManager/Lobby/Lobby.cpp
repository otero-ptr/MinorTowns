#include "Lobby.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/UUID.h"
#include "User/User.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Stringifier.h"

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
		user->messagePool.pushBackMessage(this->lobbyData);
	}
}

void Lobby::leave(std::shared_ptr<User> user)
{
	auto it = std::find(this->users.begin(), this->users.end(), user);
	if (it != this->users.end()) {
		this->users.erase(it);
	}
	this->createLobbyData();
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
	Poco::JSON::Object json;
	json.set("uuid_lobby", this->uuid);
	json.set("users_now", this->users.size());
	json.set("users_max", this->maxUser);
	std::ostringstream oss;
	Poco::JSON::Stringifier::stringify(json, oss);
	this->lobbyData = oss.str();
}

void Lobby::createUUID()
{
	Poco::UUIDGenerator generator;
	Poco::UUID uuid = generator.create();
	this->uuid = uuid.toString();
}

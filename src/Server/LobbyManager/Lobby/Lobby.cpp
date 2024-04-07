#include "Lobby.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/UUID.h"

Lobby::Lobby(int countUser)
{
	this->countUser = countUser;
	this->users.reserve(countUser);
	Poco::UUIDGenerator generator;
	Poco::UUID uuid = generator.create(); // Создание случайного UUID
	this->unique = uuid.toString();
}

Lobby::~Lobby()
{
}

void Lobby::join(std::shared_ptr<User> user)
{
	if (this->users.size() < this->countUser) {
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
	return this->users.size() == this->countUser;
}

std::vector<std::shared_ptr<User>>& Lobby::getUsers()
{
	return this->users;
}

const std::string Lobby::getUnique()
{
	return this->unique;
}

LobbyInfo Lobby::getLobbyInfo()
{
	return LobbyInfo(this->unique, this->countUser, this->users.size());
}

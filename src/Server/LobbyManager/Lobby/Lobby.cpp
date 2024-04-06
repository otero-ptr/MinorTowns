#include "Lobby.h"
#include "Lobby.h"
#include "Server\User\User.h"
#include <memory>

Lobby::Lobby(int countUser)
{
	this->countUser = countUser;
	this->users.reserve(countUser);
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
	this->users;
}

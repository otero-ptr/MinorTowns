#include "LobbyManager.h"
#include "Lobby\Lobby.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Stringifier.h"

LobbyManager::LobbyManager()
{
}

LobbyManager::~LobbyManager()
{
}

std::string LobbyManager::createLobby(int count)
{
	std::unique_ptr<Lobby> lobby = std::make_unique<Lobby>(count);
	std::string unique(lobby->getUnique());
	this->lobbies.insert(std::make_pair(unique, std::move(lobby)));
	this->refreshListLobby();
	return unique;
}

void LobbyManager::joinLobby(std::string unique, std::shared_ptr<User> user)
{
	this->lobbies.at(unique)->join(user);
	this->refreshListLobby();
}

void LobbyManager::leaveLobby(std::string unique, std::shared_ptr<User> user)
{
	this->lobbies.at(unique)->leave(user);
	this->refreshListLobby();
}

const std::string LobbyManager::getListLobby()
{
	return this->listLobby;
}

void LobbyManager::refreshListLobby()
{
	Poco::JSON::Array jsonArray;
	for (auto it = this->lobbies.begin(); it != this->lobbies.end(); ++it) {

		Poco::JSON::Object obj;
		obj.set("uuid_lobby", it->second->getUnique());
		obj.set("users_now", it->second->getNowUsers());
		obj.set("users_max", it->second->getMaxUsers());
		jsonArray.add(obj);
	}
	std::ostringstream oss;
	Poco::JSON::Stringifier::stringify(jsonArray, oss);
	this->listLobby = oss.str();
}

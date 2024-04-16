#include "LobbyManager.h"
#include "Lobby/Lobby.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/Dynamic/Var.h"

LobbyManager::LobbyManager()
{
	this->refreshListLobby();
}

LobbyManager::~LobbyManager()
{
}

std::string LobbyManager::createLobby(int count)
{
	std::unique_ptr<Lobby> lobby = std::make_unique<Lobby>(count);
	std::string uuid(lobby->getUUID());
	this->lobbies.insert(std::make_pair(uuid, std::move(lobby)));
	this->refreshListLobby();
	return uuid;
}

void LobbyManager::joinLobby(std::string uuidLobby, std::shared_ptr<User> user)
{
	if (user->getLocation() != Location::MENU) {
		return;
	}
	this->lobbies.at(uuidLobby)->join(user);
	user->setLocation(Location::LOBBY, uuidLobby);
	this->refreshListLobby();
}

void LobbyManager::leaveLobby(std::shared_ptr<User> user)
{
	if (user->getLocation() != Location::LOBBY) {
		return;
	}
	this->lobbies.at(user->getUUIDLocation())->leave(user);
	user->setLocation(Location::MENU, "menu");
	this->refreshListLobby();
}

const std::string LobbyManager::getListLobby()
{
	return this->listLobby;
}

bool LobbyManager::isLobbyFull(std::string uuidLobby)
{
	return this->lobbies.at(uuidLobby)->isFull();
}

std::vector<std::shared_ptr<User>> LobbyManager::getLobbyUsers(std::string uuidLobby)
{
	return std::move(this->lobbies.at(uuidLobby)->getUsers());
}

void LobbyManager::closeLobby(std::string uuidLobby)
{
	this->lobbies.erase(uuidLobby);
}

void LobbyManager::refreshListLobby()
{
	Poco::JSON::Array jsonArray;
	Poco::JSON::Parser parser;
	for (auto it = this->lobbies.begin(); it != this->lobbies.end(); ++it) {
		Poco::Dynamic::Var result = parser.parse(it->second->getLobbyData());
		Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
		jsonArray.add(object);
	}
	std::ostringstream oss;
	Poco::JSON::Stringifier::stringify(jsonArray, oss);
	this->listLobby = oss.str();
}

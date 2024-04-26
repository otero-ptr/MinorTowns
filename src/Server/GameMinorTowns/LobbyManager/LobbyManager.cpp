#include "LobbyManager.h"
#include "Lobby/Lobby.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/Dynamic/Var.h"

LobbyManager::LobbyManager()
{
	this->active = true;
	this->thRefresher = std::jthread(&LobbyManager::refreshListLobby, this);
	this->notifier = std::make_unique<LobbyUpdateNotifier>([this]() {return this->listLobby; });
}

LobbyManager::~LobbyManager()
{
	this->active = false;
	if (this->thRefresher.joinable()) {
		this->thRefresher.join();
	}
}

std::string LobbyManager::createLobby(int count)
{
	std::unique_ptr<Lobby> lobby = std::make_unique<Lobby>(count);
	std::string uuid(lobby->getUUID());
	this->lobbies.insert(std::make_pair(uuid, std::move(lobby)));
	return uuid;
}

void LobbyManager::joinLobby(std::string uuidLobby, std::shared_ptr<User> user)
{
	if (user->getLocation() != Location::MENU) {
		return;
	}
	this->lobbies.at(uuidLobby)->join(user);
	user->setLocation(Location::LOBBY, uuidLobby);
}

void LobbyManager::leaveLobby(std::shared_ptr<User> user)
{
	if (user->getLocation() != Location::LOBBY) {
		return;
	}
	this->lobbies.at(user->getUUIDLocation())->leave(user);
	user->setLocation(Location::MENU, "menu");
}

bool LobbyManager::isLobbyFull(std::string uuidLobby)
{
	return this->lobbies.at(uuidLobby)->isFull();
}

bool LobbyManager::isLobbyEmpty(std::string uuidLobby)
{
	return this->lobbies.at(uuidLobby)->isEmpty();
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
	while (this->active) {
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
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	}
}

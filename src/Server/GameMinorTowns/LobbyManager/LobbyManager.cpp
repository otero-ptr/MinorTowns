#include "LobbyManager.h"
#include "Lobby/Lobby.h"
#include "nlohmann/json.hpp"

LobbyManager::LobbyManager(int cooldownRefresher)
{
	this->cooldownRefresher = cooldownRefresher;
	this->active = true;
	this->thRefresher = std::jthread(&LobbyManager::refreshListLobby, this);
	this->notifier = std::make_unique<LobbyUpdateNotifier>([this]() { return this->listLobby; });
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
		nlohmann::json jsonObj;
		for (auto it = this->lobbies.begin(); it != this->lobbies.end(); ++it) {
			jsonObj["lobbies"].push_back(nlohmann::json::parse(it->second->getLobbyData()));
		}
		this->listLobby = jsonObj.dump();
		std::this_thread::sleep_for(std::chrono::milliseconds(this->cooldownRefresher));
	}
}

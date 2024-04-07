#include "LobbyManager.h"
#include "Lobby\Lobby.h"

LobbyManager::LobbyManager()
{
}

LobbyManager::~LobbyManager()
{
}

void LobbyManager::createLobby(int count)
{
}

void LobbyManager::joinLobby(std::shared_ptr<User> user)
{
}

std::vector<LobbyInfo> LobbyManager::getListLobby()
{
	std::vector<LobbyInfo> lobbyInfo;
	lobbyInfo.reserve(this->lobbies.size());
	for (auto it = this->lobbies.begin(); it != this->lobbies.end(); ++it) {
		lobbyInfo.push_back(it->second->getLobbyInfo());
	}
	return lobbyInfo;
}

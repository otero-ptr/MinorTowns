#pragma once
#include <vector>
#include <map>
#include <memory>
#include "../User/User.h"
#include "Lobby/Lobby.h"

class LobbyManager {
public:
	LobbyManager();
	~LobbyManager();
	void createLobby(int count);
	void joinLobby(std::shared_ptr<User> user);
	std::vector<LobbyInfo> getListLobby();
private:
	std::map<int, std::unique_ptr<Lobby>> lobbies;
};
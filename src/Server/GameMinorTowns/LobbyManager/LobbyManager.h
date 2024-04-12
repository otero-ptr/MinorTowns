#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "User/User.h"
#include "Lobby/Lobby.h"

class LobbyManager {
public:
	LobbyManager();
	~LobbyManager();
	std::string createLobby(int count);
	void joinLobby(std::string uuidLobby, std::shared_ptr<User> user);
	void leaveLobby(std::shared_ptr<User> user);
	const std::string getListLobby();
private:
	void refreshListLobby();
	std::string listLobby;
	std::unordered_map<std::string, std::unique_ptr<Lobby>> lobbies;
};
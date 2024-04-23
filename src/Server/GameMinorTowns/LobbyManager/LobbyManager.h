#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "User/User.h"
#include "Lobby/Lobby.h"
#include "LobbyUpdateNotifier\LobbyUpdateNotifier.h"

class LobbyManager {
public:
	LobbyManager();
	~LobbyManager();
	std::string createLobby(int count);
	void joinLobby(std::string uuidLobby, std::shared_ptr<User> user);
	void leaveLobby(std::shared_ptr<User> user);
	bool isLobbyFull(std::string uuidLobby);
	void closeLobby(std::string uuidLobby);
	std::vector<std::shared_ptr<User>> getLobbyUsers(std::string uuidLobby);
	std::unique_ptr<LobbyUpdateNotifier> notifier;
private:
	void refreshListLobby();
	std::string listLobby;
	std::unordered_map<std::string, std::unique_ptr<Lobby>> lobbies;
	bool active;
	std::jthread thRefresher;
};
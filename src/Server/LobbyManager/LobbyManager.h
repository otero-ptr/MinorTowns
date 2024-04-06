#pragma once
#include <vector>
#include <map>
#include <memory>
class Lobby;

class LobbyManager {
public:
	LobbyManager();
	~LobbyManager();
	void createLobby(int count);
	void joinLobby();
	void getListLobby();
private:
	std::map<int, std::unique_ptr<Lobby>> lobbies;
};
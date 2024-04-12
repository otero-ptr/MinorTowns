#pragma once
#include <memory>
class LobbyManager;
class GameManager;

class GameMinorTowns {
public:
	GameMinorTowns();
	std::unique_ptr<GameManager> gameManager;
	std::unique_ptr<LobbyManager> lobbyManager;
private:
};
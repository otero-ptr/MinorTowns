#pragma once
#include <memory>
#include "GameManager\GameManager.h"
#include "LobbyManager\LobbyManager.h"
#include "Middleware\Middleware.h"

class Server {
public:
	Server();
	~Server();
	void run();
private:
	void init();
	std::unique_ptr<GameManager> gameManager;
	std::unique_ptr<LobbyManager> lobbyManager;
	std::unique_ptr<Middleware> middleware;
};
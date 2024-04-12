#pragma once
#include <memory>
#include "GameManager\GameManager.h"
#include "LobbyManager\LobbyManager.h"
#include "Middleware\Middleware.h"
#include "WebSocketServer\WebSocketServer.h"

class Server {
public:
	Server();
	~Server();
	void run();
private:
	void init();
	std::unique_ptr<GameManager> gameManager;
	std::unique_ptr<LobbyManager> lobbyManager;
	std::shared_ptr<Middleware> middleware;
	std::unique_ptr<WebSocketServer> socketServer;
};
#pragma once
#include <memory>
#include "Middleware\Middleware.h"
#include "WebSocketServer\WebSocketServer.h"
#include "GameMinorTowns\GameMinorTowns.h"

class Server {
public:
	Server();
	~Server();
	void run();
private:
	void init();
	std::unique_ptr<GameMinorTowns> gameMinorTowns;
	std::shared_ptr<Middleware> middleware;
	std::unique_ptr<WebSocketServer> socketServer;
};
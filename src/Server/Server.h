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
	std::shared_ptr<GameMinorTowns> game_minor_towns;
	std::shared_ptr<Middleware> middleware;
	std::unique_ptr<WebSocketServer> socket_server;
};
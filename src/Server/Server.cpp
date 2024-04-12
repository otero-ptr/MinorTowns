#include "Server.h"

Server::Server()
{
	this->init();
}

Server::~Server()
{
}

void Server::run()
{
	this->socketServer->run();
}

void Server::init()
{
	this->gameMinorTowns = std::make_shared<GameMinorTowns>();
	this->middleware = std::make_shared<Middleware>(this->gameMinorTowns);
	this->socketServer = std::make_unique<WebSocketServer>(8080, 100);
	this->socketServer->setMiddleware(this->middleware);
}

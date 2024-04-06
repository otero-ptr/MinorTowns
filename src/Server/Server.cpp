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

}

void Server::init()
{
	this->gameManager = std::make_unique<GameManager>();
	this->lobbyManager = std::make_unique<LobbyManager>();
	this->middleware = std::make_unique<Middleware>();
}

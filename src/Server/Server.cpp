#include "Server.h"
#include "yaml-cpp\yaml.h"

Server::Server()
{
	YAML::Node config = YAML::LoadFile("server.yml");
	this->game_minor_towns = std::make_shared<GameMinorTowns>(
		config["max_users"].as<int>(), config["manager"]["game"]["collector"].as<int>(), 
		config["manager"]["lobby"]["refresher"].as<int>(), 
		config["manager"]["redis_uri"].as<std::string>());

	this->middleware = std::make_shared<Middleware>(this->game_minor_towns);

	this->socket_server = std::make_unique<WebSocketServer>(
		config["server"]["port"].as<int>(), 
		config["server"]["max_clients"].as<int>());

	this->socket_server->setMiddleware(this->middleware);

	this->socket_server->setClientSettings(

		config["client"]["cors"].as<std::string>(), 
		config["client"]["repeat_request"].as<int>(), 
		config["client"]["timeout_response"].as<int>());
}

Server::~Server()
{
}

void Server::run()
{
	this->socket_server->run();
}
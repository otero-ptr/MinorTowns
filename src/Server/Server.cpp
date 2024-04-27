#include "Server.h"
#include "yaml-cpp\yaml.h"

Server::Server()
{
	YAML::Node config = YAML::LoadFile("server.yml");
	this->gameMinorTowns = std::make_shared<GameMinorTowns>(config["max_users"].as<int>(), config["manager"]["game"]["collector"].as<int>(), config["manager"]["lobby"]["refresher"].as<int>());
	this->middleware = std::make_shared<Middleware>(this->gameMinorTowns);
	this->socketServer = std::make_unique<WebSocketServer>(config["server"]["port"].as<int>(), config["server"]["max_clients"].as<int>());
	this->socketServer->setMiddleware(this->middleware);
	this->socketServer->setClientSettings(config["client"]["cors"].as<std::string>(), config["client"]["repeat_request"].as<int>(), config["client"]["timeout_response"].as<int>());
}

Server::~Server()
{
}

void Server::run()
{
	this->socketServer->run();
}
﻿#include <memory>
#include "WebSocketServer/WebSocketServer.h"
#include "WebSocketServer/GameMinorTowns/GameMinorTowns.h"
#include "WebSocketServer/Middleware/Middleware.h"
#include "LogManager/LogManager.h"
#include "LogManager/log.h"
#include "yaml-cpp/yaml.h"
#include <iostream>

int main() {
	Logger::LogManager log_manager;
	log_manager.initialize();
	LOGGER_INFO("START");

	std::shared_ptr<GameMinorTowns> game_minor_towns;
	std::shared_ptr<Middleware> middleware;
	std::unique_ptr<WebSocketServer> socket_server;

	YAML::Node config = YAML::LoadFile("server.yml");

	middleware = std::make_shared<Middleware>(std::make_shared<GameMinorTowns>(
		config["max_users"].as<int>(), config["manager"]["game"]["collector"].as<int>(),
		config["manager"]["lobby"]["refresher"].as<int>(),
		config["manager"]["redis_uri"].as<std::string>()));

	socket_server = std::make_unique<WebSocketServer>(
		config["server"]["port"].as<int>(),
		config["server"]["max_clients"].as<int>(),
		config["client"]["cors"].as<std::string>(),
		config["client"]["repeat_request"].as<int>(),
		config["client"]["timeout_response"].as<int>(),
		std::move(middleware));

	socket_server->runServer();

	LOGGER_INFO("END");
	log_manager.shutdown();
	return 0;
}
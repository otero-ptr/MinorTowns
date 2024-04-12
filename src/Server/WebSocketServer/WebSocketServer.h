#pragma once
#include <thread>
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"

class Middleware;

class WebSocketServer {
public:
	WebSocketServer() = delete;
	WebSocketServer(int port, int maxThreads);
	~WebSocketServer();
	void run();
	void setMiddleware(std::shared_ptr<Middleware> middleware);
private:
	void runServer();
	bool running;
	std::thread thread;

	int port;
	int maxThreads;

	std::shared_ptr<Middleware> middlewareServer;
};
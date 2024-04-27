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
	void setClientSettings(std::string cors, int repeatRequest, int timeoutResponse);
private:
	void runServer();
	bool running;
	std::thread thread;

	int port;
	int maxThreads;

	std::string cors;
	int repeatRequest; 
	int timeoutResponse;
	std::shared_ptr<Middleware> middlewareServer;
};
#pragma once
#include <thread>
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"

class Middleware;


class WebSocketServer {
public:
	WebSocketServer() = delete;
	explicit WebSocketServer(int port, int max_threads, 
		std::string cors, int repeat_request, 
		int timeout_response, std::shared_ptr<Middleware> middleware);
	~WebSocketServer();
	void run();
	void setMiddleware();
	void setClientSettings();
	void runServer();
private:
	bool running;
	std::thread thread;

	int port;
	int max_threads;

	std::string cors;
	int repeat_request; 
	int timeout_response;
	std::shared_ptr<Middleware> middleware;
};
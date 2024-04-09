#pragma once
#include <thread>
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"

class WebSocketServer {
public:
	WebSocketServer() = delete;
	WebSocketServer(int port, int maxThreads);
	~WebSocketServer();
	void run();
private:
	void runServer();
	bool running;
	std::thread thread;

	int port;
	int maxThreads;
};
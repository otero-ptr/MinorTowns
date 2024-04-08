#pragma once

class WebSocketServer {
public:
	WebSocketServer() = delete;
	WebSocketServer(int port);
	~WebSocketServer();
	void run();
private:

};
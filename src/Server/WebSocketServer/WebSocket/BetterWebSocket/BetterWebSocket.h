#pragma once
#include "Poco/Net/WebSocket.h"

class BetterWebSocket {
public:
	BetterWebSocket() = delete;
	BetterWebSocket(Poco::Net::WebSocket ws, int timeout);
	~BetterWebSocket();
	std::string receiveFrame(int& flags, bool &timeout);
	int sendFrame(const std::string &msg, int flags, bool &timeout);
	void close();
private:
	int receiveFrame(void* buffer, int length, int& flags); // default
	int sendFrame(const void* buffer, int length, int flags); // default
	Poco::Net::WebSocket ws;
};

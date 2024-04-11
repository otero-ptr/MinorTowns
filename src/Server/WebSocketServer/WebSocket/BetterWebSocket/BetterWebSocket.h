#pragma once
#include "Poco/Net/WebSocket.h"

class BetterWebSocket {
public:
	BetterWebSocket() = delete;
	BetterWebSocket(Poco::Net::WebSocket ws, int timeout);
	~BetterWebSocket();
	int receiveFrame(void* buffer, int length, int& flags);
	int receiveFrame(void* buffer, int length, int& flags, bool &timeout);
	int sendFrame(const void* buffer, int length, int flags);
	int sendFrame(const void* buffer, int length, int flags, bool &timeout);
	void shutdown();
private:
	Poco::Net::WebSocket ws;
};

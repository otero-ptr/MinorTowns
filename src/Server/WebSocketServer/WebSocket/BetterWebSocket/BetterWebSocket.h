#pragma once
#include "Poco/Net/WebSocket.h"
#include "ServerMessages/PushMessage.h"
#include "ServerMessages/ResponseMessage.h"

struct BetterFrame {
	std::string text;
	int flags;
	bool timeout;
	bool frame;
};

class BetterWebSocket {
public:
	BetterWebSocket() = delete;
	explicit BetterWebSocket(Poco::Net::WebSocket ws, int timeout_sec);
	~BetterWebSocket();
	BetterFrame receiveFrame();
	int sendFrame(const std::string &msg, int flags, bool &timeout);
	int sendResponseMessage(ResponseMessage response);
	int sendPushMessage(PushMessage push, bool& timeout);
	void close();
private:
	int receiveFrame(void* buffer, int length, int& flags); // default
	int sendFrame(const void* buffer, int length, int flags); // default
	Poco::Net::WebSocket ws;
};

#pragma once
#include "Poco/Net/WebSocket.h"
#include "server_messages\push_message.h"
#include "server_messages\response_message.h"

class BetterWebSocket {
public:
	BetterWebSocket() = delete;
	BetterWebSocket(Poco::Net::WebSocket ws, int timeout);
	~BetterWebSocket();
	std::string receiveFrame(int& flags, bool &timeout);
	int sendFrame(const std::string &msg, int flags, bool &timeout);
	int sendResponseMessage(ResponseMessage response);
	int sendPushMessage(PushMessage push, bool& timeout);
	void close();
private:
	int receiveFrame(void* buffer, int length, int& flags); // default
	int sendFrame(const void* buffer, int length, int flags); // default
	Poco::Net::WebSocket ws;
};

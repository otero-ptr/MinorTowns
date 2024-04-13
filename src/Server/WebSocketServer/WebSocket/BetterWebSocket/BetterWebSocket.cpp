#include "BetterWebSocket.h"

BetterWebSocket::BetterWebSocket(Poco::Net::WebSocket ws, int timeout_sec) : ws(ws)
{
	this->ws.setReceiveTimeout(Poco::Timespan(timeout_sec, 0));
	this->ws.setSendTimeout(Poco::Timespan(timeout_sec, 0));
}

BetterWebSocket::~BetterWebSocket()
{
}

std::string BetterWebSocket::receiveFrame(int& flags, bool& timeout)
{
	char buffer[1024];
	int n;
	try {
		n = this->ws.receiveFrame(buffer, sizeof(buffer), std::ref(flags));
	}
	catch (Poco::TimeoutException& e) {
		timeout = true;
		return std::string(); // return empty string, equal timeout;
	}
	timeout = false;
	if (n < 1) {
		return std::string();
	}
	buffer[n] = '\0';
	return std::string(buffer);
}

int BetterWebSocket::receiveFrame(void* buffer, int length, int& flags)
{
	return this->ws.receiveFrame(buffer, length, std::ref(flags));
}

int BetterWebSocket::sendFrame(const void* buffer, int length, int flags)
{
	return this->ws.sendFrame(buffer, std::ref(length), std::ref(flags));
}

int BetterWebSocket::sendFrame(const std::string &msg, int flags, bool& timeout)
{
	try {
		timeout = false;
		return this->ws.sendFrame(msg.c_str(), msg.size(), std::ref(flags));
	}
	catch (Poco::TimeoutException& e) {
		timeout = true;
		return 0;
	}
}

void BetterWebSocket::shutdown()
{
	this->ws.shutdown();
}

#include "BetterWebSocket.h"

BetterWebSocket::BetterWebSocket(Poco::Net::WebSocket ws, int timeout_sec) : ws(ws)
{
	this->ws.setReceiveTimeout(Poco::Timespan(timeout_sec, 0));
	this->ws.setSendTimeout(Poco::Timespan(timeout_sec, 0));
}

BetterWebSocket::~BetterWebSocket()
{
}

int BetterWebSocket::receiveFrame(void* buffer, int length, int& flags)
{
	return this->ws.receiveFrame(buffer, length, std::ref(flags));
}

int BetterWebSocket::receiveFrame(void *buffer, int length, int &flags, bool &timeout)
{
	int n = -1;
	try {
		n = this->ws.receiveFrame(buffer, length, std::ref(flags));
	}
	catch (Poco::TimeoutException& e) {
		timeout = true;
		return n; // return -1, equal timeout;
	}
	timeout = false;
	return n;
}

int BetterWebSocket::sendFrame(const void* buffer, int length, int flags)
{
	return this->ws.sendFrame(buffer, std::ref(length), std::ref(flags));
}

int BetterWebSocket::sendFrame(const void* buffer, int length, int flags, bool &timeout)
{
	int n = -1;
	try {
		n = this->ws.sendFrame(buffer, std::ref(length), std::ref(flags));
	}
	catch (Poco::TimeoutException& e) {
		timeout = true;
		return n; // return -1, equal timeout;
	}
	timeout = false;
	return n;
}

void BetterWebSocket::shutdown()
{
	this->ws.shutdown();
}

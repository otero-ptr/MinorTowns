#include "BetterWebSocket.h"
#include <nlohmann\json.hpp>

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

int BetterWebSocket::sendResponseMessage(ResponseMessage response)
{
	try {
		nlohmann::json response_msg;
		response_msg["code"] = static_cast<int>(response.code);
		if (!response.body.empty()){
			response_msg["body"] = response.body;
		}
		std::string result_response = response_msg.dump();
		return this->ws.sendFrame(result_response.c_str(), result_response.size(), Poco::Net::WebSocket::FRAME_TEXT);
	}
	catch (Poco::TimeoutException& e) {
		return 0;
	}
	catch (...) {
		throw;
	}
}

int BetterWebSocket::sendPushMessage(PushMessage push, bool& timeout)
{
	try {
		timeout = false;
		nlohmann::json response_msg;
		nlohmann::json msg = nlohmann::json::parse(push.body);
		response_msg["msg"] = msg;
		std::string result_response = response_msg.dump();
		return this->ws.sendFrame(result_response.c_str(), result_response.size(), Poco::Net::WebSocket::FRAME_TEXT);
	}
	catch (Poco::TimeoutException& e) {
		timeout = true;
		return 0;
	}
	catch (...) {
		throw;
	}
}

void BetterWebSocket::close()
{
	this->ws.close();
}

#include "WebSocketRequestHandler.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/WebSocket.h"
#include "BetterWebSocket\BetterWebSocket.h"
#include <iostream>
#include "User\User.h"
#include "Middleware\Middleware.h"

std::string msg1("Hello! Welcome in server MinorTowns. To be continue you need send your nickname :)");
std::string msg2("Timeout. Server closed :(");
std::string msg3("Wrong user. Server closed :(");
std::string test_msg("Test msg");

void WebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	try
	{
		std::shared_ptr<User> user;
		std::string msg;
		int flags;
		bool timeout = false;
		std::cout << "WebSocket connection established." << std::endl;
		BetterWebSocket bws(Poco::Net::WebSocket(request, response), 15);
		bws.sendFrame(msg1, Poco::Net::WebSocket::FRAME_TEXT, timeout);
		msg = bws.receiveFrame(flags, timeout);
		if (timeout) {
			bws.sendFrame(msg2, Poco::Net::WebSocket::FRAME_TEXT, timeout);
			bws.shutdown();
		}
		if (!msg.empty()) {
			user = this->middlewareServer->Authorization(msg, request.clientAddress().host().toString(), request.clientAddress().port());
			if (user == nullptr) {
				bws.sendFrame(msg3, Poco::Net::WebSocket::FRAME_TEXT, timeout);
				bws.shutdown();
			}
		}
		do
		{
			msg = bws.receiveFrame(flags, timeout);
			if (!timeout) {
				if (!msg.empty()) {
					this->middlewareServer->action(msg, user);
					//bws.sendFrame(msg, Poco::Net::WebSocket::FRAME_TEXT, timeout);
				}
			}
		} while ((flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) != Poco::Net::WebSocket::FRAME_OP_CLOSE);
		std::cout << "WebSocket connection closed." << std::endl;
	}
	catch (Poco::Net::WebSocketException& exc)
	{
		std::cout << exc.message() << std::endl;
		switch (exc.code())
		{
		case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
			response.set("Sec-WebSocket-Version", Poco::Net::WebSocket::WEBSOCKET_VERSION);
			// fallthrough
		case Poco::Net::WebSocket::WS_ERR_NO_HANDSHAKE:
		case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
		case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
			response.setContentLength(0);
			response.send();
			break;
		}
	}
}

Poco::Net::HTTPRequestHandler* WebSocketRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
	return new WebSocketRequestHandler(this->middlewareServer);
}

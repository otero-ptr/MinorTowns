#include "WebSocketRequestHandler.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/WebSocket.h"
#include "BetterWebSocket\BetterWebSocket.h"
#include <iostream>

void WebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	try
	{
		char buffer[2048];
		int flags;
		int n;
		bool timeout = false;
		std::cout << "WebSocket connection established." << std::endl;
		BetterWebSocket bws(Poco::Net::WebSocket(request, response), 15);
		std::string str("Hello! Welcome in server MinorTowns. To be continue you need send your nickname :)");
		std::string str2("Timeout. Server closed :(");
		bws.sendFrame(str.c_str(), str.size(), Poco::Net::WebSocket::FRAME_TEXT, timeout);
		n = bws.receiveFrame(buffer, sizeof(buffer), flags, timeout);
		if (timeout) {
			bws.sendFrame(str2.c_str(), str2.size(), Poco::Net::WebSocket::FRAME_TEXT, timeout);
			bws.shutdown();
		}
		do
		{
			try {
				n = bws.receiveFrame(buffer, sizeof(buffer), flags, timeout);
			}
			catch (Poco::TimeoutException& e) {
				std::cerr << "Timeout occurred: " << e.what() << std::endl;
				bws.sendFrame(str2.c_str(), str2.size(), Poco::Net::WebSocket::FRAME_TEXT, timeout);
				bws.shutdown();
			}
			if (n > 0) {
				
			}
			std::cout << Poco::format("Frame received (length=%d, flags=0x%x).", n, unsigned(flags)) << std::endl;
			bws.sendFrame(buffer, n, Poco::Net::WebSocket::FRAME_TEXT, timeout);
		} while (n > 0 && (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) != Poco::Net::WebSocket::FRAME_OP_CLOSE);
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
	std::cout << "end" << std::endl;
}

Poco::Net::HTTPRequestHandler* WebSocketRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
	return new WebSocketRequestHandler;
}

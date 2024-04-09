#include "WebSocketRequestHandler.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Net/NetException.h"
#include <iostream>

void WebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	try
	{
		Poco::Net::WebSocket ws(request, response);
		std::cout << "WebSocket connection established." << std::endl;
		char buffer[1024];
		int flags;
		int n;
		do
		{
			n = ws.receiveFrame(buffer, sizeof(buffer), flags);
			std::cout << Poco::format("Frame received (length=%d, flags=0x%x).", n, unsigned(flags)) << std::endl;
			ws.sendFrame(buffer, n, flags);
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
}

Poco::Net::HTTPRequestHandler* WebSocketRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
	return new WebSocketRequestHandler;
}

#include "WebSocketRequestHandler.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/URI.h"
#include "BetterWebSocket\BetterWebSocket.h"
#include <iostream>
#include "User\User.h"
#include "Middleware\Middleware.h"
#include "request_handler\request_handler.h"

void WebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	try
	{
		std::shared_ptr<User> user;
		std::string msg;
		int flags;
		bool timeout = false;
		response.set("Access-Control-Allow-Origin", *this->cors);
		Poco::URI uri(request.getURI());
		Poco::URI::QueryParameters params = uri.getQueryParameters();
		if (params.size() != 1) {
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
			response.setContentLength(0);
			response.send();
			return;
		}
		if (params.begin()->first != "username" || params.begin()->second.empty()) {
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
			response.setContentLength(0);
			response.send();
			return;
		}
		std::cout << "WebSocket connection established." << std::endl;
		BetterWebSocket bws(Poco::Net::WebSocket(request, response), *this->timeout_response);
		msg = params.begin()->second;
		if (!msg.empty()) {
			user = this->middleware_server->authorization(msg, request.clientAddress().host().toString(), request.clientAddress().port());
			if (user == nullptr) {
				bws.sendResponseMessage(ResponseMessage(Code::NotAcceptable, "Wrong user. Server closed"));
				bws.close();
				std::cout << "WebSocket connection closed." << std::endl;
				return;
			}
			else {
				bws.sendResponseMessage(ResponseMessage(Code::Accepted, "Hello! Welcome in server MinorTowns"));
			}
		}
		bool sendThreadRun = true;
		std::jthread sendThread([&user, &bws, &sendThreadRun, repeatRequest = this->repeat_request]() {
				bool timeout = false;
				do {
					if (!user->message_pool.isEmpty()) {
						bws.sendPushMessage(user->message_pool.popFrontMessage(), timeout);
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(*repeatRequest));
				} while (sendThreadRun);
			});

		do
		{
			msg = bws.receiveFrame(flags, timeout);
			if (!timeout) {
				if (!msg.empty()) {
					auto result_handler = this->request_handler->Handler(msg);
					if (result_handler.has_value()) {
						if (!result_handler->isError()) {
							auto result_middleware = this->middleware_server->action(std::move(*result_handler), user);
							if (result_middleware.first == MIDDLEWARE_STATUS::ST_OK) {
								bws.sendResponseMessage(ResponseMessage(Code::OK, result_middleware.second));
							}
							else if (result_middleware.first == MIDDLEWARE_STATUS::ST_ERROR) {
								bws.sendResponseMessage(ResponseMessage(Code::InternalServerError, result_middleware.second));
							}
						} else {
							bws.sendResponseMessage(ResponseMessage(Code::InternalServerError, result_handler->getErrorInfo()->err_info));
						}
					} else {
						bws.sendResponseMessage(ResponseMessage(Code::NotFound));
					}
				}
			}
		} while ((flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) != Poco::Net::WebSocket::FRAME_OP_CLOSE);
		if (sendThread.joinable()) {
			sendThreadRun = false;
			sendThread.join();
		}
		this->middleware_server->disconnect(user);
		bws.close();
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
			break;
		case Poco::Net::WebSocket::WS_ERR_NO_HANDSHAKE:
			break;
		case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
			break;
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
	return new WebSocketRequestHandler(this->middleware_server, this->cors, this->repeat_request, this->timeout_response);
}

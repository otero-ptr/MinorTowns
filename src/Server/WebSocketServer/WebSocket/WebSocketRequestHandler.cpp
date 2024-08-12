#include "WebSocketRequestHandler.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/URI.h"
#include <iostream>
#include "Middleware\Middleware.h"
#include "RequestHandler\RequestHandler.h"

WebSocketRequestHandler::~WebSocketRequestHandler()
{
	if (th_request.joinable()) {
		th_request.request_stop();
		th_request.join();
	}
	middleware_server->disconnect(user);
	user->message_pool.clear();
	bws->close();
	std::cout << "WebSocket connection closed." << std::endl;
}

void WebSocketRequestHandler::handleRequest(
	Poco::Net::HTTPServerRequest& request, 
	Poco::Net::HTTPServerResponse& response) {
	try
	{
		response.set("Access-Control-Allow-Origin", cors.data());
		Poco::URI uri(request.getURI());
		if (!handleConnect(response, uri.getQueryParameters())) {
			return;
		}
		std::cout << "WebSocket connection established." << std::endl;
		bws = std::make_unique<BetterWebSocket>(Poco::Net::WebSocket(request, response), timeout_response);
		std::string username = uri.getQueryParameters().begin()->second;
		if (!username.empty()) {
			user = this->middleware_server->authorization(username, 
				request.clientAddress().host().toString(), 
				request.clientAddress().port());
			if (user == nullptr) {
				bws->sendResponseMessage(ResponseMessage(Code::NotAcceptable, "Wrong user. Server closed"));
				bws->close();
				return;
			}
			else {
				bws->sendResponseMessage(ResponseMessage(Code::Accepted, "Hello! Welcome in server MinorTowns"));
			}
		}
		runRequestThread();
		while (true) {
			auto [text, flags, timeout, frame] = bws->receiveFrame();
			if (!frame) {
				continue;
			}
			if (timeout) {
				break;
			}
			if (flags == Poco::Net::WebSocket::FRAME_OP_CLOSE) {
				break;
			}
			if (!timeout) {
				if (!text.empty()) {
					auto result_handler = request_handler->Handler(text);
					if (result_handler.has_value()) {
						if (!result_handler->isError()) {
							auto result_middleware = middleware_server->action(std::move(*result_handler), user);
							if (result_middleware.first == MIDDLEWARE_STATUS::ST_OK) {
								bws->sendResponseMessage(ResponseMessage(Code::OK, result_middleware.second));
							}
							else if (result_middleware.first == MIDDLEWARE_STATUS::ST_ERROR) {
								bws->sendResponseMessage(ResponseMessage(Code::InternalServerError, result_middleware.second));
							}
						} else {
							bws->sendResponseMessage(ResponseMessage(Code::InternalServerError, result_handler->getErrorInfo()->err_info));
						}
					} else {
						bws->sendResponseMessage(ResponseMessage(Code::NotFound));
					}
				}
			}
		}
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
	catch (...) {
		std::cout << "something else" << std::endl;
	}
}

Poco::Net::HTTPRequestHandler* WebSocketRequestHandlerFactory::createRequestHandler(
	const Poco::Net::HTTPServerRequest& request)
{
	return new WebSocketRequestHandler(this->middleware_server, this->cors, this->repeat_request, this->timeout_response);
}

bool WebSocketRequestHandler::handleConnect(
	Poco::Net::HTTPServerResponse& response, 
	const Poco::URI::QueryParameters& params) {
		bool status = true;
		if (params.size() != 1) {
			return false;
		}
		if (params.begin()->first != "username" || params.begin()->second.empty()) {
			return false;
		}
		if (!status) {
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
			response.setContentLength(0);
			response.send();
		}
		return status;
}

void WebSocketRequestHandler::runRequestThread()
{
	th_request = std::jthread([this](std::stop_token token) {
		bool timeout = false;
		while (!token.stop_requested()) {
			if (!user->message_pool.empty()) {
				bws->sendPushMessage(user->message_pool.pop(), timeout);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(repeat_request));
		};
	});
}

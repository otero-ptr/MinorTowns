#include "WebSocketRequestHandler.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/URI.h"
#include "log.h"
#include "User.h"
#include "Middleware.h"
#include "RequestHandler.h"

WebSocketRequestHandler::~WebSocketRequestHandler()
{
	if (th_server_messages.joinable()) {
		th_server_messages.request_stop();
		th_server_messages.join();
	}
	if (user != nullptr) {
		middleware_server->disconnect(user);
		user->message_pool.clear();
		bws->close();
	}
	LOGGER_INFO("WebSocket connection closed");
}

void WebSocketRequestHandler::handleRequest(
	Poco::Net::HTTPServerRequest& request, 
	Poco::Net::HTTPServerResponse& response) {
	try
	{
		LOGGER_INFO("WebSocket connection established");
		if (!connectionAuthorization(request, response)) {
			LOGGER_ERROR("WebSocket connection is not authorized");
			return ;
		} else {
			LOGGER_INFO("Client: " + request.clientAddress().toString());
			LOGGER_INFO("username: " + user->getUsername() + " uuid: " + user->getUUID());
		}
		runProcessingServerMessagesThread();
		runProcessingClientMessages();
	}
	catch (Poco::Net::WebSocketException& exc)
	{
		LOGGER_ERROR("Ecxeption code: " + std::to_string(exc.code()) + " msg: " + exc.message());
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
		LOGGER_ERROR("some problem")
	}
}

Poco::Net::HTTPRequestHandler* WebSocketRequestHandlerFactory::createRequestHandler(
	const Poco::Net::HTTPServerRequest& request)
{
	return new WebSocketRequestHandler(this->middleware_server, this->cors, this->repeat_request, this->timeout_response);
}

bool WebSocketRequestHandler::connectionAuthorization(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	response.set("Access-Control-Allow-Origin", cors.data());
	Poco::URI uri(request.getURI());
	if (!handleParams(response, uri.getQueryParameters())) {
		LOGGER_ERROR("Parameters are missing");
		return false;
	}
	bws = std::make_unique<BetterWebSocket>(Poco::Net::WebSocket(request, response), timeout_response);
	std::string username = uri.getQueryParameters().begin()->second;
	if (!username.empty()) {
		user = this->middleware_server->authorization(username,
			request.clientAddress().host().toString(),
			request.clientAddress().port());
		if (user == nullptr) {
			bws->sendResponseMessage(ResponseMessage(Code::NotAcceptable, "Wrong user"));
			bws->close();
			return false;
		}
		else {
			bws->sendResponseMessage(ResponseMessage(Code::Accepted, "Hello! Welcome in server MinorTowns"));
		}
	}
	return true;
}

bool WebSocketRequestHandler::handleParams(
	Poco::Net::HTTPServerResponse& response, 
	const Poco::URI::QueryParameters& params) {
		if (params.size() != 1) {
			LOGGER_TRACE("Invalid number of parameters");
			return false;
		}
		if (params.begin()->first != "username" || params.begin()->second.empty()) {
			LOGGER_TRACE("Parameters are incorrect");
			return false;
		}
		return true;
}

void WebSocketRequestHandler::runProcessingServerMessagesThread()
{
	th_server_messages = std::jthread([this](std::stop_token token) {
		bool timeout = false;
		while (!token.stop_requested()) {
			if (!user->message_pool.empty()) {
				bws->sendPushMessage(user->message_pool.pop(), timeout);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(repeat_request));
		};
	});
}

void WebSocketRequestHandler::runProcessingClientMessages()
{
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
				auto result_handler = RequestHandler::Handler(text);
				if (std::holds_alternative<RequestResult>(result_handler)) {
					auto result_middleware = middleware_server->action(std::move(std::get<RequestResult>(result_handler)), user);
					if (result_middleware.first == MIDDLEWARE_STATUS::ST_OK) {
						bws->sendResponseMessage(ResponseMessage(Code::OK, result_middleware.second));
					}
					else if (result_middleware.first == MIDDLEWARE_STATUS::ST_ERROR) {
						bws->sendResponseMessage(ResponseMessage(Code::InternalServerError, result_middleware.second));
					}
				}
				else if (std::holds_alternative<RequestError>(result_handler)){
					bws->sendResponseMessage(ResponseMessage(Code::InternalServerError, std::get<RequestError>(result_handler).err));
				}
				else {
					bws->sendResponseMessage(ResponseMessage(Code::NotFound));
				}
			}
		}
	}
}

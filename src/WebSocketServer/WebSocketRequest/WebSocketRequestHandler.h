#pragma once
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "RequestHandler/RequestHandler.h"
#include "Poco/URI.h"
#include "User/User.h"
#include "BetterWebSocket/BetterWebSocket.h"

class Middleware;

class WebSocketRequestHandler : public Poco::Net::HTTPRequestHandler {
public:
    WebSocketRequestHandler(std::shared_ptr<Middleware> middleware,
        const std::string &cors, const int &repeat_request, const int &timeout_response) 
        : cors(cors), repeat_request(repeat_request), 
        timeout_response(timeout_response), middleware_server(middleware), 
        request_handler(std::make_unique<RequestHandler>()) {

    }
    ~WebSocketRequestHandler();
    void handleRequest(Poco::Net::HTTPServerRequest& request, 
        Poco::Net::HTTPServerResponse& response);
private:
    bool connectionAuthorization(Poco::Net::HTTPServerRequest& request,
        Poco::Net::HTTPServerResponse& response);
    bool handleParams(Poco::Net::HTTPServerResponse& response, 
        const Poco::URI::QueryParameters& params);

    void runProcessingServerMessagesThread();
    void runProcessingClientMessages();
    const std::string_view cors;
    const int repeat_request;
    const int timeout_response;
    std::shared_ptr<Middleware> middleware_server;
    std::unique_ptr<RequestHandler> request_handler;

    std::shared_ptr<User> user;
    std::unique_ptr<BetterWebSocket> bws;

    std::jthread th_server_messages;
};

class WebSocketRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
    WebSocketRequestHandlerFactory(std::shared_ptr<Middleware> middleware, 
        std::string cors, int repeat_request, int timeout_response) 
        : middleware_server(middleware),
        cors(cors), repeat_request(repeat_request), timeout_response(timeout_response) {
    }
    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);
private:
    std::string cors;
    int repeat_request;
    int timeout_response;
    std::shared_ptr<Middleware> middleware_server;
};
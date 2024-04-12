#pragma once
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"

class Middleware;

class WebSocketRequestHandler : public Poco::Net::HTTPRequestHandler {
public:
    WebSocketRequestHandler(std::shared_ptr<Middleware> middleware) : middlewareServer(middleware) {}
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
private:
    std::shared_ptr<Middleware> middlewareServer;
};

class WebSocketRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
    WebSocketRequestHandlerFactory(std::shared_ptr<Middleware> middleware) : middlewareServer(middleware) {}
    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);
private:
    std::shared_ptr<Middleware> middlewareServer;
};
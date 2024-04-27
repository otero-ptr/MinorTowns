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
    WebSocketRequestHandler(std::shared_ptr<Middleware> middleware,const std::string &cors,const int &repeatRequest,const int &timeoutResponse) : middlewareServer(middleware) {
        this->cors = &cors;
        this->repeatRequest = &repeatRequest;
        this->timeoutResponse = &timeoutResponse;
    }
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
private:
    const std::string *cors;
    const int *repeatRequest;
    const int *timeoutResponse;
    std::shared_ptr<Middleware> middlewareServer;
};

class WebSocketRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
    WebSocketRequestHandlerFactory(std::shared_ptr<Middleware> middleware, std::string cors, int repeatRequest, int timeoutResponse) : middlewareServer(middleware),
        cors(cors), repeatRequest(repeatRequest), timeoutResponse(timeoutResponse) {}
    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);
private:
    std::string cors;
    int repeatRequest;
    int timeoutResponse;
    std::shared_ptr<Middleware> middlewareServer;
};
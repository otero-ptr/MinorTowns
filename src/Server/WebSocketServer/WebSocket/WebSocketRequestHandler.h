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
class Middleware;

class WebSocketRequestHandler : public Poco::Net::HTTPRequestHandler {
public:
    WebSocketRequestHandler(std::shared_ptr<Middleware> middleware,const std::string &cors,const int &repeat_request,const int &timeout_response) 
        : middleware_server(middleware), request_handler(std::make_unique<RequestHandler>()) {
        this->cors = &cors;
        this->repeat_request = &repeat_request;
        this->timeout_response = &timeout_response;
    }
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
private:
    const std::string *cors;
    const int *repeat_request;
    const int *timeout_response;
    std::shared_ptr<Middleware> middleware_server;
    std::unique_ptr<RequestHandler> request_handler;
};

class WebSocketRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
    WebSocketRequestHandlerFactory(std::shared_ptr<Middleware> middleware, std::string cors, int repeat_request, int timeout_response) 
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
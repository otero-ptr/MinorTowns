#include "WebSocketServer.h"
#include "WebSocketServer.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "WebSocketRequest/WebSocketRequestHandler.h"
#include <iostream>
#include <thread>


WebSocketServer::WebSocketServer(int port, int max_threads, 
    std::string cors, int repeat_request, int timeout_response, 
    std::shared_ptr<Middleware> middleware)
    : port(port), max_threads(max_threads), 
    cors(cors), repeat_request(repeat_request),
    timeout_response(timeout_response), middleware(middleware) {

}

WebSocketServer::~WebSocketServer() {

}

void WebSocketServer::runServer()
{
    if (middleware == nullptr) return;

    Poco::Net::HTTPServerParams* params = new Poco::Net::HTTPServerParams;
    params->setMaxThreads(max_threads);
    Poco::Net::ServerSocket svs(this->port);
    Poco::Net::HTTPServer srv(new WebSocketRequestHandlerFactory(middleware, cors, repeat_request, timeout_response), svs, params);

    srv.start();
    while (true) {
        Poco::Thread::sleep(1000);
    }
    srv.stop();
}

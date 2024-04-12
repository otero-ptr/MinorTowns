#include "WebSocketServer.h"
#include "WebSocketServer.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "WebSocket/WebSocketRequestHandler.h"
#include <iostream>
#include <thread>

WebSocketServer::WebSocketServer(int port, int maxThreads)
{
    this->port = port;
    this->maxThreads = maxThreads;
    this->running = false;
}

WebSocketServer::~WebSocketServer()
{
    if (this->running) {
        this->thread.join();
    }
}

void WebSocketServer::run()
{
    if (this->middlewareServer == nullptr) return;
    this->thread = std::thread(&WebSocketServer::runServer, this);  
    this->running = true;
}

void WebSocketServer::setMiddleware(std::shared_ptr<Middleware> middleware)
{
    if (middleware == nullptr) return;
    this->middlewareServer = middleware;
}

void WebSocketServer::runServer()
{
    Poco::Net::HTTPServerParams* params = new Poco::Net::HTTPServerParams;
    params->setMaxThreads(this->maxThreads);
    Poco::Net::ServerSocket svs(this->port);
    Poco::Net::HTTPServer srv(new WebSocketRequestHandlerFactory(this->middlewareServer), svs, params);
    srv.start();
    std::cout << "Server started" << std::endl;
    std::cout << "Press Ctrl-C to stop the server" << std::endl;
    while (true) {
        Poco::Thread::sleep(1000);
    }
    srv.stop();
}

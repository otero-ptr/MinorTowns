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
    this->thread = std::thread(&WebSocketServer::runServer, this);  
    this->running = true;
}

void WebSocketServer::runServer()
{
    Poco::Net::HTTPServerParams* params = new Poco::Net::HTTPServerParams;
    params->setMaxThreads(this->maxThreads);
    Poco::Net::ServerSocket svs(this->port);
    Poco::Net::HTTPServer srv(new WebSocketRequestHandlerFactory, svs, params);
    srv.start();
    std::cout << "Server started" << std::endl;
    std::cout << "Press Ctrl-C to stop the server" << std::endl;
    while (true) {
        Poco::Thread::sleep(1000);
    }
    srv.stop();
}

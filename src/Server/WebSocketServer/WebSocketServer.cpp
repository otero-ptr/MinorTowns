#include "WebSocketServer.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "WebSocket/WebSocketRequestHandler.h"
#include "WebSocket/WebSocketRequestHandlerFactory.h"
#include "Poco/Net/ServerSocket.h"
#include <iostream>

WebSocketServer::WebSocketServer(int port)
{
}

WebSocketServer::~WebSocketServer()
{
}

void WebSocketServer::run()
{
    Poco::Net::HTTPServerParams* params = new Poco::Net::HTTPServerParams;
    params->setMaxThreads(100);
    Poco::Net::ServerSocket svs(8080);
    Poco::Net::HTTPServer srv(new WebSocketRequestHandlerFactory, svs, params);
    srv.start();
    std::cout << "Server started" << std::endl;
    std::cout << "Press Ctrl-C to stop the server" << std::endl;
    while (true) {
        Poco::Thread::sleep(1000);
    }
    srv.stop();
}

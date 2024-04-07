#include <iostream>
#include "Server/Server.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Server\LobbyManager\Lobby\Lobby.h"
//
//using namespace Poco;
//using namespace Poco::Net;
////using namespace Poco::Util;
//
//class WebSocketRequestHandler : public HTTPRequestHandler {
//public:
//    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
//        std::cout << "New connect" << std::endl;
//
//        response.setStatus(HTTPResponse::HTTP_OK);
//        response.setContentType("text/html");
//        std::ostream& ostr = response.send();
//        ostr << "<html><head><title>MinorTowns WebSocket Server</title></head>";
//        ostr << "<body><h1>WebSocket Server</h1>";
//        ostr << "<body><h1>MinorTowns</h1>";
//        ostr << "</body></html>";
//
//        WebSocket ws(request, response);
//        char buffer[1024];
//        int flags;
//        int n;
//        do {
//            n = ws.receiveFrame(buffer, sizeof(buffer), flags);
//            std::cout << "Received message: " << std::string(buffer, n) << std::endl;
//            ws.sendFrame(buffer, n, flags);
//        } while (n > 0 || (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
//        std::cout << "End connect" << std::endl;
//    }
//};
//
//class WebSocketRequestHandlerFactory : public HTTPRequestHandlerFactory {
//public:
//    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) {
//        return new WebSocketRequestHandler;
//    }
//};
//
//int main() {
//    HTTPServerParams* params = new HTTPServerParams;
//    params->setMaxThreads(100);
//    ServerSocket svs(8080);
//    HTTPServer srv(new WebSocketRequestHandlerFactory, svs, params);
//    srv.start();
//    std::cout << "Server started" << std::endl;
//    std::cout << "Press Ctrl-C to stop the server" << std::endl;
//    while (true) {
//        Poco::Thread::sleep(1000);
//    }
//    srv.stop();
//    return 0;
//}

int main() {
	Server server();
	std::cout << "Start" << std::endl;
	for (int i = 0; i < 10000; i++) {
		Lobby lobby1(4);
		std::cout << lobby1.getUnique() << std::endl;
	}

	return 0;
}
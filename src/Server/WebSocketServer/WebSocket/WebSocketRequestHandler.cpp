#include "WebSocketRequestHandler.h"
#include <iostream>

void WebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
    std::cout << "New connect" << std::endl;

    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("text/html");

    std::ostream& ostr = response.send();
    ostr << "<html><head><title>MinorTowns WebSocket Server</title></head>";
    ostr << "<body><h1>WebSocket Server</h1>";
    ostr << "<body><h1>MinorTowns</h1>";
    ostr << "</body></html>";

    Poco::Net::WebSocket ws(request, response);
    char buffer[1024];
    int flags;
    int n;
    do {
        n = ws.receiveFrame(buffer, sizeof(buffer), flags);
        std::cout << "Received message: " << std::string(buffer, n) << std::endl;
        ws.sendFrame(buffer, n, flags);
    } while (n > 0 || (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) != Poco::Net::WebSocket::FRAME_OP_CLOSE);
    std::cout << "End connect" << std::endl;
}

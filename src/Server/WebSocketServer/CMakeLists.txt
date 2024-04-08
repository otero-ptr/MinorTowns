add_subdirectory("WebSocket")
set(WEBSOCKET_SERVER_FILES
    "WebSocketServer.cpp"
    "WebSocketServer.h"
)

list(APPEND WEBSOCKET_SERVER_FILES ${WEBSOCKET_FILES})
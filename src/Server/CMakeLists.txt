add_subdirectory("GameManager")
add_subdirectory("LobbyManager")
add_subdirectory("Middleware")
add_subdirectory("User")
add_subdirectory("WebSocketServer")

set(SERVER_FILES
    "Server.cpp"
    "Server.h"
)

list(APPEND SERVER_FILES ${WEBSOCKET_SERVER_FILES})
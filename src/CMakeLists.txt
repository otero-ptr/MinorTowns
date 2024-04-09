# CMakeList.txt : CMake project for MinorTowns, include source and define
# project specific logic here.
#

# Find and include Poco
find_package(Poco REQUIRED COMPONENTS Foundation Net Util JSON)
include_directories(${Poco_INCLUDE_DIRS})

# Find and include ZLIB
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

add_subdirectory("Server")

set(SRC_FILES)

set(MINOR_TOWNS_FILES
    "MinorTowns.cpp"
    "MinorTowns.h"
)

set(SERVER_FILES
    "Server/Server.cpp"
    "Server/Server.h"
)

set(WEBSOCKET_SERVER_FILES
    "Server/WebSocketServer/WebSocketServer.cpp"
    "Server/WebSocketServer/WebSocketServer.h"
)

set(WEBSOCKET_FILES
    "Server/WebSocketServer/WebSocket/WebSocketRequestHandler.cpp"
    "Server/WebSocketServer/WebSocket/WebSocketRequestHandler.h"
)

set(USER_FILES
    "Server/User/User.cpp"
    "Server/User/User.h"
)

set(MIDDLEWARE_FILES
    "Server/Middleware/Middleware.cpp"
    "Server/Middleware/Middleware.h"
)

set(LOBBY_MANAGER_FILES
    "Server/LobbyManager/LobbyManager.cpp"
    "Server/LobbyManager/LobbyManager.h"
)

set(LOBBY_FILES
    "Server/LobbyManager/Lobby/Lobby.cpp"
    "Server/LobbyManager/Lobby/Lobby.h"
)

set(GAME_MANAGER_FILES
    "Server/GameManager/GameManager.cpp"
    "Server/GameManager/GameManager.h"
)

set(GAME_FILES
    "Server/GameManager/Game/Game.cpp"
    "Server/GameManager/Game/Game.h"
)

list(APPEND SRC_FILES ${USER_FILES})
list(APPEND SRC_FILES ${GAME_FILES})
list(APPEND SRC_FILES ${GAME_MANAGER_FILES})
list(APPEND SRC_FILES ${LOBBY_FILES})
list(APPEND SRC_FILES ${MIDDLEWARE_FILES})
list(APPEND SRC_FILES ${LOBBY_MANAGER_FILES})
list(APPEND SRC_FILES ${WEBSOCKET_FILES})
list(APPEND SRC_FILES ${WEBSOCKET_SERVER_FILES})
list(APPEND SRC_FILES ${MINOR_TOWNS_FILES})
list(APPEND SRC_FILES ${SERVER_FILES})

# Add source files to the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})


# Link Poco and ZLIB libraries to the executable
target_link_libraries(${PROJECT_NAME} PUBLIC ${Poco_LIBRARIES} ${ZLIB_LIBRARIES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MinorTowns PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

# CMakeList.txt : CMake project for MinorTowns, include source and define
# project specific logic here.
#

# Find and include Poco
find_package(Poco REQUIRED COMPONENTS Foundation Net Util JSON)
include_directories(${Poco_INCLUDE_DIRS})

# Find and include ZLIB
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

include_directories(Server/)

add_subdirectory("Server")

set(MINOR_TOWNS_FILES
    "MinorTowns.cpp"
    "MinorTowns.h"
)
list(APPEND SRC_FILES ${MINOR_TOWNS_FILES})

set(SERVER_FILES
    "Server/Server.cpp"
    "Server/Server.h"
)
list(APPEND SRC_FILES ${SERVER_FILES})

set(USER_FILES
    "Server/User/User.cpp"
    "Server/User/User.h"
)
list(APPEND SRC_FILES ${USER_FILES})

set(MESSAGE_POOL_FILES
    "Server/User/MessagePool/MessagePool.h" 
    "Server/User/MessagePool/MessagePool.cpp"
)
list(APPEND SRC_FILES ${MESSAGE_POOL_FILES})

set(WEBSOCKET_SERVER_FILES
    "Server/WebSocketServer/WebSocketServer.cpp"
    "Server/WebSocketServer/WebSocketServer.h"
)
list(APPEND SRC_FILES ${WEBSOCKET_SERVER_FILES})

set(BETTER_WEBSOCKET_FILES
    "Server/WebSocketServer/WebSocket/BetterWebSocket/BetterWebSocket.h" 
    "Server/WebSocketServer/WebSocket/BetterWebSocket/BetterWebSocket.cpp"
)
list(APPEND SRC_FILES ${BETTER_WEBSOCKET_FILES})

set(WEBSOCKET_FILES
    "Server/WebSocketServer/WebSocket/WebSocketRequestHandler.cpp"
    "Server/WebSocketServer/WebSocket/WebSocketRequestHandler.h"
)
list(APPEND SRC_FILES ${WEBSOCKET_FILES})

set(GAME_MINOR_TOWNS_FILES
    "Server/GameMinorTowns/GameMinorTowns.h"
    "Server/GameMinorTowns/GameMinorTowns.cpp"
)
list(APPEND SRC_FILES ${GAME_MINOR_TOWNS_FILES})

set(LOBBY_MANAGER_FILES
    "Server/GameMinorTowns/LobbyManager/LobbyManager.cpp"
    "Server/GameMinorTowns/LobbyManager/LobbyManager.h"
)
list(APPEND SRC_FILES ${LOBBY_MANAGER_FILES})

set(LOBBY_FILES
    "Server/GameMinorTowns/LobbyManager/Lobby/Lobby.cpp"
    "Server/GameMinorTowns/LobbyManager/Lobby/Lobby.h"
)
list(APPEND SRC_FILES ${LOBBY_FILES})

set(GAME_MANAGER_FILES
    "Server/GameMinorTowns/GameManager/GameManager.cpp"
    "Server/GameMinorTowns/GameManager/GameManager.h"
)
list(APPEND SRC_FILES ${GAME_MANAGER_FILES})

set(GAME_FILES
    "Server/GameMinorTowns/GameManager/Game/Game.cpp"
    "Server/GameMinorTowns/GameManager/Game/Game.h"
)
list(APPEND SRC_FILES ${GAME_FILES})

set(GAME_MAP_FILES
    "Server/GameMinorTowns/GameManager/Game/GameMap/GameMap.cpp"
    "Server/GameMinorTowns/GameManager/Game/GameMap/GameMap.h"
)
list(APPEND SRC_FILES ${GAME_MAP_FILES})

set(NODE_MAP_FILES 
    "Server/GameMinorTowns/GameManager/Game/GameMap/NodeMap/NodeMap.h" 
    "Server/GameMinorTowns/GameManager/Game/GameMap/NodeMap/NodeMap.cpp"
)
list(APPEND SRC_FILES ${NODE_MAP_FILES})

set(TOWN_FILES 
    "Server/GameMinorTowns/GameManager/Game/Town/Town.h"
    "Server/GameMinorTowns/GameManager/Game/Town/Town.cpp"
)
list(APPEND SRC_FILES ${TOWN_FILES})

set(ECONOMY_FILES 
    "Server/GameMinorTowns/GameManager/Game/Town/Economy/Economy.h"
    "Server/GameMinorTowns/GameManager/Game/Town/Economy/Economy.cpp"
)
list(APPEND SRC_FILES ${ECONOMY_FILES})

set(BUILDINGS_FILES 
    "Server/GameMinorTowns/GameManager/Game/Town/Economy/Buildings/Buildings.h" 
    "Server/GameMinorTowns/GameManager/Game/Town/Economy/Buildings/Buildings.cpp"
)
list(APPEND SRC_FILES ${BUILDINGS_FILES})

set(MARKET_FILES 
    "Server/GameMinorTowns/GameManager/Game/Town/Economy/Market/Market.h" 
    "Server/GameMinorTowns/GameManager/Game/Town/Economy/Market/Market.cpp" 
)
list(APPEND SRC_FILES ${MARKET_FILES})

set(MIDDLEWARE_FILES
    "Server/Middleware/Middleware.cpp"
    "Server/Middleware/Middleware.h"
)
list(APPEND SRC_FILES ${MIDDLEWARE_FILES})

set(AUTH_VALIDATE_FILES
    "Server/Middleware/AuthorizatonValidate/AuthorizationValidate.h"
    "Server/Middleware/AuthorizatonValidate/AuthorizationValidate.cpp"
)
list(APPEND SRC_FILES ${AUTH_VALIDATE_FILES})

# Add source files to the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})


# Link Poco and ZLIB libraries to the executable
target_link_libraries(${PROJECT_NAME} PUBLIC ${Poco_LIBRARIES} ${ZLIB_LIBRARIES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MinorTowns PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

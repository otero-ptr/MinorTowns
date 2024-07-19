# CMakeList.txt : CMake project for MinorTowns, include source and define
# project specific logic here.
#

# Find and include Poco
find_package(Poco REQUIRED COMPONENTS Foundation Net Util JSON)

# Find and include ZLIB
find_package(ZLIB REQUIRED)

# Find and include YAML CPP
find_package(yaml-cpp REQUIRED)

find_package(nlohmann_json CONFIG REQUIRED)

find_package(redis++ CONFIG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(${CMAKE_SOURCE_DIR}/src/Server/)

add_subdirectory("Server")
add_subdirectory("util")

set(MINOR_TOWNS_FILES
    "MinorTowns.cpp"
    "MinorTowns.h"
)
list(APPEND SRC_FILES ${MINOR_TOWNS_FILES})

set(UTIL_FILES
    "util/gen_uuid.h"
    "util/gen_uuid.cpp"
)
list(APPEND SRC_FILES ${UTIL_FILES})

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

set(LOBBY_UPDATE_NOTIFIER_FILES
    "Server/GameMinorTowns/LobbyManager/LobbyUpdateNotifier/LobbyUpdateNotifier.h"
    "Server/GameMinorTowns/LobbyManager/LobbyUpdateNotifier/LobbyUpdateNotifier.cpp"
)
list(APPEND SRC_FILES ${LOBBY_UPDATE_NOTIFIER_FILES})

set(GAME_MANAGER_FILES
    "Server/GameMinorTowns/GameManager/GameManager.cpp"
    "Server/GameMinorTowns/GameManager/GameManager.h"
)
list(APPEND SRC_FILES ${GAME_MANAGER_FILES})

set(GAME_FILES
    "Server/GameMinorTowns/GameManager/Game/Game.h" 
    "Server/GameMinorTowns/GameManager/Game/Game.cpp"
)
list(APPEND SRC_FILES ${GAME_FILES})

set(ARMY_FILES
    "Server/GameMinorTowns/GameManager/Game/Army/Army.h" 
    "Server/GameMinorTowns/GameManager/Game/Army/Army.cpp"
)
list(APPEND SRC_FILES ${ARMY_FILES})

set(BATTLE_FILES
    "Server/GameMinorTowns/GameManager/Game/Army/Battle/Battle.h"
    "Server/GameMinorTowns/GameManager/Game/Army/Battle/Battle.cpp"
)
list(APPEND SRC_FILES ${BATTLE_FILES})

set(GAME_SETTINGS_FILES
    "Server/GameMinorTowns/GameManager/Game/GameSettings/GameSettings.h" 
    "Server/GameMinorTowns/GameManager/Game/GameSettings/GameSettings.cpp"
)
list(APPEND SRC_FILES ${GAME_SETTINGS_FILES})

set(GAME_CONTROLLER_FILES
    "Server/GameMinorTowns/GameManager/Game/GameController/GameController.h" 
    "Server/GameMinorTowns/GameManager/Game/GameController/GameController.cpp"
)
list(APPEND SRC_FILES ${GAME_CONTROLLER_FILES})

set(GAME_MAP_FILES
    "Server/GameMinorTowns/GameManager/Game/GameMap/GameMap.cpp"
    "Server/GameMinorTowns/GameManager/Game/GameMap/GameMap.h"
)
list(APPEND SRC_FILES ${GAME_MAP_FILES})

set(NODE_MAP_FILES 
    "Server/GameMinorTowns/GameManager/Game/GameMap/NodeMap/NodeMap.h" 
    "Server/GameMinorTowns/GameManager/Game/GameMap/NodeMap/NodeMap.cpp"
    "Server/GameMinorTowns/GameManager/Game/GameMap/NodeMap/NodeType.h" 
    "Server/GameMinorTowns/GameManager/Game/GameMap/NodeMap/NodeMapTown.h" 
    "Server/GameMinorTowns/GameManager/Game/GameMap/NodeMap/NodeMapTown.cpp"
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
    "Server/GameMinorTowns/GameManager/Game/Town/Buildings/Buildings.h" 
    "Server/GameMinorTowns/GameManager/Game/Town/Buildings/Buildings.cpp"
)
list(APPEND SRC_FILES ${BUILDINGS_FILES})

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


set(ACTION_MANAGER_FILES
"Server/GameMinorTowns/GameManager/Game/ActionManager/ActionManager.h" 
"Server/GameMinorTowns/GameManager/Game/ActionManager/ActionManager.cpp" 
)
list(APPEND SRC_FILES ${ACTION_MANAGER_FILES})

set(ACTION_FILES
"Server/GameMinorTowns/GameManager/Game/ActionManager/Action/Action.h" 
"Server/GameMinorTowns/GameManager/Game/ActionManager/Action/Action.cpp" 
)
list(APPEND SRC_FILES ${ACTION_FILES})

set(ACTION_LAST_FILES
"Server/GameMinorTowns/GameManager/Game/ActionManager/Action/ActionLast/ActionLast.h" 
"Server/GameMinorTowns/GameManager/Game/ActionManager/Action/ActionLast/ActionLast.cpp" 
)
list(APPEND SRC_FILES ${ACTION_LAST_FILES})

set(ACTION_EVERY_FILES
"Server/GameMinorTowns/GameManager/Game/ActionManager/Action/ActionEvery/ActionEvery.h" 
"Server/GameMinorTowns/GameManager/Game/ActionManager/Action/ActionEvery/ActionEvery.cpp" 
)
list(APPEND SRC_FILES ${ACTION_EVERY_FILES})

set(ACTION_EVERY_FILES
"Server/GameMinorTowns/GameManager/Game/ActionManager/Action/ActionEvery/ActionEvery.h" 
"Server/GameMinorTowns/GameManager/Game/ActionManager/Action/ActionEvery/ActionEvery.cpp" 
)
list(APPEND SRC_FILES ${ACTION_EVERY_FILES})

set(ACTION_FOREVER_FILES
"Server/GameMinorTowns/GameManager/Game/ActionManager/Action/ActionForever/ActionForever.h" 
"Server/GameMinorTowns/GameManager/Game/ActionManager/Action/ActionForever/ActionForever.cpp" 
)
list(APPEND SRC_FILES ${ACTION_FOREVER_FILES})

set(REQUEST_HANDLER_FILES
"Server/request_handler/request_handler.h" 
"Server/request_handler/request_handler.cpp" 
"Server/request_handler/params.h" 
"Server/request_handler/error_info.h" 
"Server/request_handler/request_result.h"  
"Server/request_handler/params_validator.h" 
"Server/request_handler/params_validator.cpp"
)
list(APPEND SRC_FILES ${REQUEST_HANDLER_FILES})

# sanitizer ASan
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
# set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")

# sanitizer MSan
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory -g")
# set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=memory")

# sanitizer TSan
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -g")
# set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=thread")

# sanitizer UBSan
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -g")
# set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=undefined")

# Add source files to the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})


# Link Poco and ZLIB libraries to the executable
target_link_libraries(${PROJECT_NAME} PUBLIC ${Poco_LIBRARIES} ${ZLIB_LIBRARIES} ${YAML_CPP_LIBRARIES} nlohmann_json::nlohmann_json redis++::redis++)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MinorTowns PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

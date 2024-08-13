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

find_package(spdlog CONFIG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(${CMAKE_SOURCE_DIR}/src/WebSocketServer/)
include_directories(${CMAKE_SOURCE_DIR}/src/LogManager/)

add_subdirectory("WebSocketServer")
add_subdirectory("util")
add_subdirectory("LogManager")

set(MINOR_TOWNS_FILES
    "MinorTowns.cpp"
    "MinorTowns.h"
)
list(APPEND SRC_FILES ${MINOR_TOWNS_FILES})

set(WEBSOCKET_SERVER_FILES
    "WebSocketServer/WebSocketServer.cpp"
    "WebSocketServer/WebSocketServer.h"
)
list(APPEND SRC_FILES ${WEBSOCKET_SERVER_FILES})

set(LOG_MANAGER_FILES
    "LogManager/LogManager.h" 
    "LogManager/LogManager.cpp" 
    "LogManager/log.h"
)
list(APPEND SRC_FILES ${LOG_MANAGER_FILES})

set(UTIL_FILES
    "util/gen_uuid.h"
    "util/gen_uuid.cpp"
)
list(APPEND SRC_FILES ${UTIL_FILES})

set(USER_FILES
    "WebSocketServer/User/User.cpp"
    "WebSocketServer/User/User.h"
)
list(APPEND SRC_FILES ${USER_FILES})

set(MESSAGE_POOL_FILES
    "WebSocketServer/User/MessagePool/MessagePool.h" 
    "WebSocketServer/User/MessagePool/MessagePool.cpp"
)
list(APPEND SRC_FILES ${MESSAGE_POOL_FILES})

set(BETTER_WEBSOCKET_FILES
    "WebSocketServer/WebSocketRequest/BetterWebSocket/BetterWebSocket.h" 
    "WebSocketServer/WebSocketRequest/BetterWebSocket/BetterWebSocket.cpp"
)
list(APPEND SRC_FILES ${BETTER_WEBSOCKET_FILES})

set(SERVER_MESSAGES_FILES
    "WebSocketServer/WebSocketRequest/BetterWebSocket/ServerMessages/HttpStatusCodes.h"
    "WebSocketServer/WebSocketRequest/BetterWebSocket/ServerMessages/PushMessage.h"
    "WebSocketServer/WebSocketRequest/BetterWebSocket/ServerMessages/ResponseMessage.h"
)
list(APPEND SRC_FILES ${SERVER_MESSAGES_FILES})

set(WEBSOCKET_FILES
    "WebSocketServer/WebSocketRequest/WebSocketRequestHandler.cpp"
    "WebSocketServer/WebSocketRequest/WebSocketRequestHandler.h"
)
list(APPEND SRC_FILES ${WEBSOCKET_FILES})

set(GAME_MINOR_TOWNS_FILES
    "WebSocketServer/GameMinorTowns/GameMinorTowns.h"
    "WebSocketServer/GameMinorTowns/GameMinorTowns.cpp"
)
list(APPEND SRC_FILES ${GAME_MINOR_TOWNS_FILES})

set(LOBBY_MANAGER_FILES
    "WebSocketServer/GameMinorTowns/LobbyManager/LobbyManager.cpp"
    "WebSocketServer/GameMinorTowns/LobbyManager/LobbyManager.h"
)
list(APPEND SRC_FILES ${LOBBY_MANAGER_FILES})

set(LOBBY_UPDATE_NOTIFIER_FILES
    "WebSocketServer/GameMinorTowns/LobbyManager/LobbyUpdateNotifier/LobbyUpdateNotifier.h"
    "WebSocketServer/GameMinorTowns/LobbyManager/LobbyUpdateNotifier/LobbyUpdateNotifier.cpp"
)
list(APPEND SRC_FILES ${LOBBY_UPDATE_NOTIFIER_FILES})

set(GAME_MANAGER_FILES
    "WebSocketServer/GameMinorTowns/GameManager/GameManager.cpp"
    "WebSocketServer/GameMinorTowns/GameManager/GameManager.h"
)
list(APPEND SRC_FILES ${GAME_MANAGER_FILES})

set(GAME_FILES
    "WebSocketServer/GameMinorTowns/GameManager/Game/Game.h" 
    "WebSocketServer/GameMinorTowns/GameManager/Game/Game.cpp"
)
list(APPEND SRC_FILES ${GAME_FILES})

set(ARMY_FILES
    "WebSocketServer/GameMinorTowns/GameManager/Game/Army/Army.h" 
    "WebSocketServer/GameMinorTowns/GameManager/Game/Army/Army.cpp"
)
list(APPEND SRC_FILES ${ARMY_FILES})

set(BATTLE_FILES
    "WebSocketServer/GameMinorTowns/GameManager/Game/Army/Battle/Battle.h"
    "WebSocketServer/GameMinorTowns/GameManager/Game/Army/Battle/Battle.cpp"
)
list(APPEND SRC_FILES ${BATTLE_FILES})

set(GAME_SETTINGS_FILES
    "WebSocketServer/GameMinorTowns/GameManager/GameSettings/GameSettings.h" 
    "WebSocketServer/GameMinorTowns/GameManager/GameSettings/GameSettings.cpp"
)
list(APPEND SRC_FILES ${GAME_SETTINGS_FILES})

set(GAME_CONTROLLER_FILES
    "WebSocketServer/GameMinorTowns/GameManager/Game/GameController/GameController.h" 
    "WebSocketServer/GameMinorTowns/GameManager/Game/GameController/GameController.cpp"
    "WebSocketServer/GameMinorTowns/GameManager/Game/GameController/GameNotify.h"
)
list(APPEND SRC_FILES ${GAME_CONTROLLER_FILES})

set(GAME_MAP_FILES
    "WebSocketServer/GameMinorTowns/GameManager/Game/GameMap/GameMap.cpp"
    "WebSocketServer/GameMinorTowns/GameManager/Game/GameMap/GameMap.h"
)
list(APPEND SRC_FILES ${GAME_MAP_FILES})

set(NODE_MAP_FILES 
    "WebSocketServer/GameMinorTowns/GameManager/Game/GameMap/NodeMap/NodeMap.h" 
    "WebSocketServer/GameMinorTowns/GameManager/Game/GameMap/NodeMap/NodeMap.cpp"
    "WebSocketServer/GameMinorTowns/GameManager/Game/GameMap/NodeMap/NodeType.h" 
    "WebSocketServer/GameMinorTowns/GameManager/Game/GameMap/NodeMap/NodeMapTown.h" 
    "WebSocketServer/GameMinorTowns/GameManager/Game/GameMap/NodeMap/NodeMapTown.cpp"
)
list(APPEND SRC_FILES ${NODE_MAP_FILES})

set(TOWN_FILES 
    "WebSocketServer/GameMinorTowns/GameManager/Game/Town/Town.h"
    "WebSocketServer/GameMinorTowns/GameManager/Game/Town/Town.cpp"
)
list(APPEND SRC_FILES ${TOWN_FILES})

set(ECONOMY_FILES 
    "WebSocketServer/GameMinorTowns/GameManager/Game/Town/Economy/Economy.h"
    "WebSocketServer/GameMinorTowns/GameManager/Game/Town/Economy/Economy.cpp"
)
list(APPEND SRC_FILES ${ECONOMY_FILES})

set(BUILDINGS_FILES 
    "WebSocketServer/GameMinorTowns/GameManager/Game/Town/Buildings/Buildings.h"
    "WebSocketServer/GameMinorTowns/GameManager/Game/Town/Buildings/Charch.h"
    "WebSocketServer/GameMinorTowns/GameManager/Game/Town/Buildings/Manufactory.h"
    "WebSocketServer/GameMinorTowns/GameManager/Game/Town/Buildings/TypeBuildings.h"
)
list(APPEND SRC_FILES ${BUILDINGS_FILES})

set(MIDDLEWARE_FILES
    "WebSocketServer/Middleware/Middleware.cpp"
    "WebSocketServer/Middleware/Middleware.h"
)
list(APPEND SRC_FILES ${MIDDLEWARE_FILES})

set(AUTH_VALIDATE_FILES
    "WebSocketServer/Middleware/AuthorizatonValidate/AuthorizationValidate.h"
    "WebSocketServer/Middleware/AuthorizatonValidate/AuthorizationValidate.cpp"
)
list(APPEND SRC_FILES ${AUTH_VALIDATE_FILES})


set(ACTION_MANAGER_FILES
    "WebSocketServer/GameMinorTowns/GameManager/Game/ActionManager/ActionManager.h" 
    "WebSocketServer/GameMinorTowns/GameManager/Game/ActionManager/ActionManager.cpp" 
)
list(APPEND SRC_FILES ${ACTION_MANAGER_FILES})

set(ACTION_FILES
    "WebSocketServer/GameMinorTowns/GameManager/Game/ActionManager/Action/Action.h" 
    "WebSocketServer/GameMinorTowns/GameManager/Game/ActionManager/Action/Action.cpp" 
)
list(APPEND SRC_FILES ${ACTION_FILES})

set(ACTION_LAST_FILES
    "WebSocketServer/GameMinorTowns/GameManager/Game/ActionManager/Action/ActionLast/ActionLast.h" 
    "WebSocketServer/GameMinorTowns/GameManager/Game/ActionManager/Action/ActionLast/ActionLast.cpp" 
)
list(APPEND SRC_FILES ${ACTION_LAST_FILES})

set(ACTION_EVERY_FILES
    "WebSocketServer/GameMinorTowns/GameManager/Game/ActionManager/Action/ActionEvery/ActionEvery.h" 
    "WebSocketServer/GameMinorTowns/GameManager/Game/ActionManager/Action/ActionEvery/ActionEvery.cpp" 
)
list(APPEND SRC_FILES ${ACTION_EVERY_FILES})

set(ACTION_EVERY_FILES
    "WebSocketServer/GameMinorTowns/GameManager/Game/ActionManager/Action/ActionEvery/ActionEvery.h" 
    "WebSocketServer/GameMinorTowns/GameManager/Game/ActionManager/Action/ActionEvery/ActionEvery.cpp" 
)
list(APPEND SRC_FILES ${ACTION_EVERY_FILES})

set(ACTION_FOREVER_FILES
    "WebSocketServer/GameMinorTowns/GameManager/Game/ActionManager/Action/ActionForever/ActionForever.h" 
    "WebSocketServer/GameMinorTowns/GameManager/Game/ActionManager/Action/ActionForever/ActionForever.cpp" 
)
list(APPEND SRC_FILES ${ACTION_FOREVER_FILES})

set(REQUEST_HANDLER_FILES
    "WebSocketServer/RequestHandler/RequestHandler.h" 
    "WebSocketServer/RequestHandler/RequestHandler.cpp" 
    "WebSocketServer/RequestHandler/Params.h" 
    "WebSocketServer/RequestHandler/ErrorInfo.h" 
    "WebSocketServer/RequestHandler/RequestResult.h"  
    "WebSocketServer/RequestHandler/ParamsValidator.h" 
    "WebSocketServer/RequestHandler/ParamsValidator.cpp"
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

configure_file(server.yml ${CMAKE_BINARY_DIR}/src/server.yml COPYONLY)

# Add source files to the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})


# Link Poco and ZLIB libraries to the executable
target_link_libraries(${PROJECT_NAME} PUBLIC ${Poco_LIBRARIES} ${ZLIB_LIBRARIES} ${YAML_CPP_LIBRARIES} nlohmann_json::nlohmann_json redis++::redis++ spdlog::spdlog)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MinorTowns PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

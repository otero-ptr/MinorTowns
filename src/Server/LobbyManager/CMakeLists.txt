add_subdirectory ("Lobby")

set(LOBBY_MANAGER_FILES
    "LobbyManager.cpp"
    "LobbyManager.h"
)

list(APPEND LOBBY_MANAGER_FILES ${LOBBY_FILES})
include_directories(${Poco_INCLUDE_DIRS})
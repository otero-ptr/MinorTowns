add_subdirectory("Game")
set(GAME_MANAGER_FILES
    "GameManager.cpp"
    "GameManager.h"
)

list(APPEND GAME_MANAGER_FILES ${GAME_FILES})
add_subdirectory("GameManager")
add_subdirectory("LobbyManager")

set(INC
	.
	${CMAKE_SOURCE_DIR}/src/WebSocketServer/User
)

set(INC_SYS
  
)

set(SRC
	GameMinorTowns.h
	GameMinorTowns.cpp
)

set(LIB
	User
	GameManager
	LobbyManager
)

add_library(GameMinorTowns STATIC ${SRC})
target_include_directories(GameMinorTowns PRIVATE ${INC} ${INC_SYS})
target_link_libraries(GameMinorTowns ${LIB})
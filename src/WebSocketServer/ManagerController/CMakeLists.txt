add_subdirectory("GameManager")
add_subdirectory("LobbyManager")

set(INC
	.
	${CMAKE_SOURCE_DIR}/src/WebSocketServer/User
)

set(INC_SYS
  
)

set(SRC
	IManagerController.h
	ManagerController.h
	ManagerController.cpp
)

set(LIB
	User
	GameManager
	LobbyManager
)

add_library(ManagerController STATIC ${SRC})
target_include_directories(ManagerController PRIVATE ${INC} ${INC_SYS})
target_link_libraries(ManagerController ${LIB})
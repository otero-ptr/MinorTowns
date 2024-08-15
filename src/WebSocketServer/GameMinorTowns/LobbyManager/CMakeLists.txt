set(INC
	.
	LobbyUpdateNotifier
	${CMAKE_SOURCE_DIR}/src/util
	${CMAKE_SOURCE_DIR}/src/WebSocketServer/User
)

set(INC_SYS

)

set(SRC
	LobbyManager.h
	LobbyManager.cpp
	LobbyUpdateNotifier/LobbyUpdateNotifier.h
	LobbyUpdateNotifier/LobbyUpdateNotifier.cpp
)

set(LIB
	redis++::redis++
	User
	Util
)

add_library(LobbyManager STATIC ${SRC})
target_include_directories(LobbyManager PRIVATE ${INC} ${INC_SYS})
target_link_libraries(LobbyManager ${LIB})

target_compile_features(LobbyManager PUBLIC cxx_std_20)
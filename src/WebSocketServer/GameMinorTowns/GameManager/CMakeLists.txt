add_subdirectory("Game")

set(INC
	.
	${CMAKE_SOURCE_DIR}/src/WebSocketServer/User
)

set(INC_SYS

)

set(SRC
	GameManager.h
	GameManager.cpp
)

set(LIB
	User
	Game
)

add_library(GameManager STATIC ${SRC})
target_include_directories(GameManager PRIVATE ${INC} ${INC_SYS})
target_link_libraries(GameManager ${LIB})

target_compile_features(GameManager PUBLIC cxx_std_20)
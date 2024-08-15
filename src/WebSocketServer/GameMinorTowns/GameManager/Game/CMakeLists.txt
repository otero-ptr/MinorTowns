add_subdirectory("Town")
add_subdirectory("GameSettings")

set(INC
	.
	GameMap
	GameMap/NodeMap

	GameController
	
	Army
	Army/Battle

	GameSettings
	${CMAKE_SOURCE_DIR}/src/util
	${CMAKE_SOURCE_DIR}/src/WebSocketServer/User
)

set(INC_SYS

)

set(SRC
	Game.h
	Game.cpp

	GameMap/GameMap.h
	GameMap/GameMap.cpp
	GameMap/NodeMap/NodeMap.h
	GameMap/NodeMap/NodeMap.cpp
	GameMap/NodeMap/NodeMapTown.h
	GameMap/NodeMap/NodeMapTown.cpp
	GameMap/NodeMap/NodeType.h

	GameController/GameController.h
	GameController/GameController.cpp
	GameController/GameNotify.h

	Army/Army.h
	Army/Army.cpp
	Army/Battle/Battle.h
	Army/Battle/Battle.cpp
)

set(LIB
	User
	GameSettings
	Town
)

add_library(Game STATIC ${SRC})
target_include_directories(Game PRIVATE ${INC} ${INC_SYS})
target_link_libraries(Game ${LIB})

target_compile_features(Game PUBLIC cxx_std_20)
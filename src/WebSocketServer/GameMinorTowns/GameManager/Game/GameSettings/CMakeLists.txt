configure_file(game.yml ${CMAKE_BINARY_DIR}/src/game.yml COPYONLY)

set(INC
	.
	../Town
)

set(INC_SYS

)

set(SRC
	GameSettings.h
	GameSettings.cpp
)

set(LIB
	${YAML_CPP_LIBRARIES}
	Town
)

add_library(GameSettings STATIC ${SRC})
target_include_directories(GameSettings PUBLIC ${INC} ${INC_SYS})
target_link_libraries(GameSettings PUBLIC ${LIB})
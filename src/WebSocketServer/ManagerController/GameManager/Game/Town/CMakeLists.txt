set(INC
	.
	Buildings
	Economy
	${CMAKE_SOURCE_DIR}/src/WebSocketServer/User
)

set(INC_SYS

)

set(SRC
	Town.h
	Town.cpp
	Buildings/Buildings.h
	Buildings/Church.h
	Buildings/Manufactory.h
	Buildings/TypeBuildings.h
	Economy/Economy.h
	Economy/Economy.cpp
)

set(LIB
	User
)

add_library(Town STATIC ${SRC})
target_include_directories(Town PRIVATE ${INC} ${INC_SYS})
target_link_libraries(Town ${LIB})

target_compile_features(Town PUBLIC cxx_std_20)
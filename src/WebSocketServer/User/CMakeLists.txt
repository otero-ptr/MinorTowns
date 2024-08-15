set(INC
	.
	MessagePool
	${CMAKE_SOURCE_DIR}/src/util
)

set(INC_SYS

)

set(SRC
	User.h
	User.cpp
	MessagePool/MessagePool.h
	MessagePool/MessagePool.cpp
)

set(LIB
	Util
)

add_library(User STATIC ${SRC})
target_include_directories(User PRIVATE ${INC} ${INC_SYS})
target_link_libraries(User ${LIB})
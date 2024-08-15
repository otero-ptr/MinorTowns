set(INC
	.
	AuthorizationValidate
	${CMAKE_SOURCE_DIR}/src/WebSocketServer/User
	${CMAKE_SOURCE_DIR}/src/WebSocketServer/GameMinorTowns
	${CMAKE_SOURCE_DIR}/src/WebSocketServer/RequestHandler
)

set(INC_SYS
  
)

set(SRC
	Middleware.h
	Middleware.cpp
	AuthorizationValidate/AuthorizationValidate.h
	AuthorizationValidate/AuthorizationValidate.cpp
)

set(LIB
	RequestHandler
	GameMinorTowns
)

add_library(Middleware STATIC ${SRC})
target_include_directories(Middleware PRIVATE ${INC} ${INC_SYS})
target_link_libraries(Middleware ${LIB})
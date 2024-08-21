add_subdirectory("ManagerController")
add_subdirectory("Middleware")
add_subdirectory("User")
add_subdirectory("WebSocketRequest")
add_subdirectory("RequestHandler")

set(INC
	.
	ManagerController
)

set(INC_SYS

)

set(SRC
	WebSocketServer.h
	WebSocketServer.cpp
)

set(LIB
	${Poco_LIBRARIES}
	Middleware
	WebSocketRequest
)

add_library(WebSocketServer STATIC ${SRC})
target_include_directories(WebSocketServer PRIVATE ${INC} ${INC_SYS})
target_link_libraries(WebSocketServer ${LIB})
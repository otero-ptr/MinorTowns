set(INC
	.
	BetterWebSocket
	BetterWebSocket/ServerMessages
	${CMAKE_SOURCE_DIR}/src/WebSocketServer/Middleware
	${CMAKE_SOURCE_DIR}/src/WebSocketServer/User
	${CMAKE_SOURCE_DIR}/src/LogManager
)

set(INC_SYS

)

set(SRC
	WebSocketRequestHandler.h
	WebSocketRequestHandler.cpp
	BetterWebSocket/BetterWebSocket.h
	BetterWebSocket/BetterWebSocket.cpp
	BetterWebSocket/ServerMessages/HttpStatusCodes.h
	BetterWebSocket/ServerMessages/PushMessage.h
	BetterWebSocket/ServerMessages/ResponseMessage.h
)

set(LIB
	${Poco_INCLUDE_DIRS}
	nlohmann_json::nlohmann_json
	User
	Middleware
	RequestHandler
	LogManager
)

add_library(WebSocketRequest STATIC ${SRC})
target_include_directories(WebSocketRequest PRIVATE ${INC} ${INC_SYS})
target_include_directories(WebSocketRequest PUBLIC 	${CMAKE_SOURCE_DIR}/src/WebSocketServer/RequestHandler)
target_link_libraries(WebSocketRequest ${LIB})
set(INC
	${CMAKE_SOURCE_DIR}/src/WebSocketServer/User
	${CMAKE_SOURCE_DIR}/src/WebSocketServer/RequestHandler
	${CMAKE_SOURCE_DIR}/src/WebSocketServer/ManagerController/GameManager/Game
	${CMAKE_SOURCE_DIR}/src/WebSocketServer/ManagerController
	${CMAKE_SOURCE_DIR}/src/WebSocketServer/Middleware
)



# make executable for tests
add_executable(${PROJECT_NAME}Tests 
test_main.cpp 
test_user.cpp 
test_request_handler.cpp
test_params_validator.cpp 
test_game_map.cpp 
mock_manager_controller.h
test_middleware.cpp)

target_include_directories(${PROJECT_NAME}Tests PRIVATE ${INC})

target_link_libraries(${PROJECT_NAME}Tests
	PRIVATE
	GTest::gtest 
	GTest::gmock
	GTest::gtest_main
	User
	RequestHandler
	Game
	Middleware)

add_test(NAME ${PROJECT_NAME}Tests COMMAND ${PROJECT_NAME}Tests)
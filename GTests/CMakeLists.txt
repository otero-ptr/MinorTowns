# make executable for tests
add_executable(${PROJECT_NAME}Tests test_main.cpp)

target_link_libraries(${PROJECT_NAME}Tests
	PRIVATE
	GTest::gtest 
	GTest::gtest_main)

# target_link_libraries(MyProjectTests PRIVATE libs)

add_test(NAME ${PROJECT_NAME}Tests COMMAND ${PROJECT_NAME}Tests)
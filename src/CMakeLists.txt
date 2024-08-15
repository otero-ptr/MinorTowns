# CMakeList.txt : CMake project for MinorTowns, include source and define
# project specific logic here.
#

find_package(Poco REQUIRED COMPONENTS Foundation Net Util)

find_package(ZLIB REQUIRED)

find_package(yaml-cpp REQUIRED)

find_package(nlohmann_json CONFIG REQUIRED)

find_package(redis++ CONFIG REQUIRED)

find_package(spdlog CONFIG REQUIRED)

add_subdirectory("WebSocketServer")
add_subdirectory("util")
add_subdirectory("LogManager")

set(MINOR_TOWNS_FILES
    "MinorTowns.cpp"
    "MinorTowns.h"
)
list(APPEND SRC_FILES ${MINOR_TOWNS_FILES})

# sanitizer ASan
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
# set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")

# sanitizer MSan
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory -g")
# set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=memory")

# sanitizer TSan
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -g")
# set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=thread")

# sanitizer UBSan
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -g")
# set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=undefined")

configure_file(server.yml ${CMAKE_BINARY_DIR}/src/server.yml COPYONLY)

# Add source files to the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})


# Link libraries to the executable
target_link_libraries(${PROJECT_NAME} PUBLIC ${YAML_CPP_LIBRARIES} WebSocketServer LogManager)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MinorTowns PROPERTY
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)
endif()

# TODO: Add tests and install targets if needed.

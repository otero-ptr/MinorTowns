# CMakeList.txt : CMake project for MinorTowns, include source and define
# project specific logic here.
#

# Find and include Poco
find_package(Poco REQUIRED COMPONENTS Foundation Net Util JSON)
include_directories(${Poco_INCLUDE_DIRS})

# Find and include ZLIB
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

add_subdirectory("Server")

set(MINOR_TOWNS_FILES
    "MinorTowns.cpp"
    "MinorTowns.h"
)

list(APPEND MINOR_TOWNS_FILES ${GAME_MANAGER_FILES})
list(APPEND MINOR_TOWNS_FILES ${LOBBY_MANAGER_FILES})
list(APPEND MINOR_TOWNS_FILES ${MIDDLEWARE_FILES})
list(APPEND MINOR_TOWNS_FILES ${USER_FILES})
list(APPEND MINOR_TOWNS_FILES ${SERVER_FILES})

# Add source files to the executable
add_executable(${PROJECT_NAME} ${MINOR_TOWNS_FILES})

# Link Poco and ZLIB libraries to the executable
target_link_libraries(${PROJECT_NAME} PUBLIC ${Poco_LIBRARIES} ${ZLIB_LIBRARIES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MinorTowns PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

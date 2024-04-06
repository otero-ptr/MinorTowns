# CMakeList.txt : CMake project for MinorTowns, include source and define
# project specific logic here.
#
set(SERVER_FILES 
    "Server/Server.h"
    "Server/Server.cpp"
    "Server/GameManager/GameManager.cpp"
    "Server/GameManager/GameManager.h"
    "Server/LobbyManager/LobbyManager.h"
    "Server/LobbyManager/LobbyManager.cpp"
    "Server/Middleware/Middleware.h"
    "Server/Middleware/Middleware.cpp"
    "Server/User/User.h"
)

set(MINOR_TOWNS_FILES
    "MinorTowns.cpp"
    "MinorTowns.h"
)


# Find and include Poco
find_package(Poco REQUIRED COMPONENTS Foundation Net Util)
include_directories(${Poco_INCLUDE_DIRS})

# Find and include ZLIB
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

# Add source files to the executable
add_executable(${PROJECT_NAME} ${MINOR_TOWNS_FILES} ${SERVER_FILES})

# Link Poco and ZLIB libraries to the executable
target_link_libraries(${PROJECT_NAME} PUBLIC ${Poco_LIBRARIES} ${ZLIB_LIBRARIES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MinorTowns PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

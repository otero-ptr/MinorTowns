add_subdirectory("GameMinorTowns")
add_subdirectory("Middleware")
add_subdirectory("User")
add_subdirectory("WebSocketServer")

configure_file(server.yml ${CMAKE_BINARY_DIR}/src/server.yml COPYONLY)
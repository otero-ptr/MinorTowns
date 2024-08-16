set(INC
	.
)

set(INC_SYS
  
)

set(SRC
	log.h
	LogManager.h
	LogManager.cpp
)

set(LIB
	spdlog::spdlog
)

add_library(LogManager STATIC ${SRC})
target_include_directories(LogManager PRIVATE ${INC} ${INC_SYS})
target_link_libraries(LogManager ${LIB})
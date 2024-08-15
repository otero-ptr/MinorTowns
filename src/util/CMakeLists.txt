set(INC
	.
)

set(INC_SYS
  
)

set(SRC
	gen_uuid.h
	gen_uuid.cpp
)

set(LIB
	${Poco_LIBRARIES}
)

add_library(Util STATIC ${SRC})
target_include_directories(Util PRIVATE ${INC} ${INC_SYS})
target_link_libraries(Util ${LIB})
set(INC
	.
)

set(INC_SYS
  
)

set(SRC
	ErrorInfo.h
	Operations.h
	Params.h
	ParamsValidator.h
	ParamsValidator.cpp
	RequestHandler.h
	RequestHandler.cpp
	RequestResult.h
)

set(LIB
	nlohmann_json::nlohmann_json
)

add_library(RequestHandler STATIC ${SRC})
target_include_directories(RequestHandler PRIVATE ${INC} ${INC_SYS})
target_link_libraries(RequestHandler ${LIB})

target_compile_features(RequestHandler PUBLIC cxx_std_20)

add_executable(${PROJECT_NAME}
	main.cpp
	language_server.cpp
	database.cpp
)

CPMAddPackage("gh:fmtlib/fmt#9.1.0")
CPMAddPackage("gh:jtv/libpqxx#7.7.5")

# Precompiled common headers
#add_library(core SHARED core.cpp)
#target_precompile_headers(core
#	PRIVATE
#		<vector>
#		<thread>
#		<chrono>
#		<regex>
#	INTERFACE
#		"core.hpp")
#
#target_link_libraries(${PROJECT_NAME} PRIVATE core)

target_link_libraries(${PROJECT_NAME} PRIVATE 
	fmt::fmt 
	pqxx
)
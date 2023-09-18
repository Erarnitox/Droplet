
add_executable(${PROJECT_NAME}
	main.cpp
	main.hpp
)

# build/link static libraries
set(BUILD_SHARED_LIBS OFF)

# third party dependencies
CPMAddPackage("gh:fmtlib/fmt#9.1.0")
CPMAddPackage("gh:jtv/libpqxx#7.7.5")

add_subdirectory(core)
add_subdirectory(commands)
# add_subdirectory(deamons)

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

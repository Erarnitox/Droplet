
add_executable(start_bot
	main.cpp
)

add_library(${PROJECT_NAME} STATIC
	bot.cpp
	bot.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC .)

target_link_libraries(start_bot PRIVATE ${PROJECT_NAME})

# build/link static libraries
set(BUILD_SHARED_LIBS OFF)

# third party dependencies
CPMAddPackage("gh:fmtlib/fmt#9.1.0")
CPMAddPackage("gh:jtv/libpqxx#7.7.5")

# generate test coverage information for debug builds
if(CMAKE_BUILD_TYPE STREQUAL Debug)
	target_compile_options(${PROJECT_NAME} PRIVATE --coverage)
	target_link_options(${PROJECT_NAME} PUBLIC --coverage)
	add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND 
		find ${CMAKE_BINARY_DIR} -type f
		-name '*.gcda' -exec rm {} +
	)
endif()
	

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

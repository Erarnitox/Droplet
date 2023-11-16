add_subdirectory(bot)
add_subdirectory(commands)

add_library(bot_library STATIC
	BotLibrary.cpp
)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
	# generate test coverage information
	target_compile_options(bot_library PRIVATE --coverage)
	target_link_options(bot_library PUBLIC --coverage)
	add_custom_command(TARGET bot_library PRE_BUILD COMMAND 
		find ${CMAKE_BINARY_DIR} -type f
		-name '*.gcda' -exec rm {} +
	)

	# format the source code
	clang_format(bot_library ${CMAKE_CURRENT_LIST_DIR})
endif()

target_include_directories(bot_library PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

target_link_libraries(bot_library PUBLIC bot)
target_link_libraries(bot_library PUBLIC commands)

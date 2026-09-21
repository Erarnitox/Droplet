add_subdirectory(bot)
add_subdirectory(persistence)
add_subdirectory(commands)
add_subdirectory(restapi)

add_library(bot_library STATIC
	BotLibrary.cpp
	Application.cpp
)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
	target_compile_options(bot_library PRIVATE --coverage)
	target_link_options(bot_library PUBLIC --coverage)
	add_custom_command(TARGET bot_library PRE_BUILD COMMAND
		find ${CMAKE_BINARY_DIR} -type f
		-name '*.gcda' -exec rm {} +
	)
	clang_format(bot_library ${CMAKE_CURRENT_LIST_DIR})
endif()

droplet_target_warnings(bot_library)
target_compile_features(bot_library PUBLIC cxx_std_23)
set_property(TARGET bot_library PROPERTY CXX_STANDARD ${CPP_STANDARD})
set_property(TARGET bot_library PROPERTY CXX_STANDARD_REQUIRED ON)
target_include_directories(bot_library PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
target_link_libraries(bot_library PUBLIC bot)
target_link_libraries(bot_library PUBLIC persistence)
target_link_libraries(bot_library PUBLIC commands)
target_link_libraries(bot_library PUBLIC restapi)

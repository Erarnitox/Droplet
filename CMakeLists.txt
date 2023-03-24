cmake_minimum_required(VERSION 3.25)

project(cyber_drop VERSION 1.0)

add_subdirectory(src)

add_subdirectory(libs)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}
)

set(LIBRARY_LINK_PATH "${CMAKE_BINARY_DIR}/libs/DPP/library/libdpp.so")
set(LIBRARY_PATH "${CMAKE_BINARY_DIR}/libs/DPP/library/libdpp.so.1.0")
set(TOKEN_PATH "${CMAKE_CURRENT_LIST_DIR}/src/bot_token.txt")

add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/libdpp.so" "${CMAKE_BINARY_DIR}/libdpp.so.1.0" "${CMAKE_BINARY_DIR}/bot_token.txt"
    POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBRARY_LINK_PATH} "${CMAKE_BINARY_DIR}/libdpp.so"
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBRARY_PATH} "${CMAKE_BINARY_DIR}/libdpp.so.1.0"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TOKEN_PATH} "${CMAKE_BINARY_DIR}/bot_token.txt"
)

add_custom_target(
    copy_depends ALL DEPENDS "${CMAKE_BINARY_DIR}/libdpp.so" "${CMAKE_BINARY_DIR}/libdpp.so.1.0" "${CMAKE_BINARY_DIR}/bot_token.txt"
)

add_dependencies(${PROJECT_NAME} copy_depends)
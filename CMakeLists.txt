cmake_minimum_required(VERSION 3.25)
include(cmake/CPM.cmake)

project(cyber_drop VERSION 1.0)

add_subdirectory(src)

add_subdirectory(libs)


set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}
)

 set(TOKEN_PATH "${CMAKE_CURRENT_LIST_DIR}/src/bot_token.txt")
 set(DB_CREDS_PATH "${CMAKE_CURRENT_LIST_DIR}/src/db_connection.txt")
 set(DPP_BINARY_PATH "${CMAKE_BINARY_DIR}/libs/DPP/library/libdpp.so.1.0")
 
 add_custom_command(
     OUTPUT "${CMAKE_BINARY_DIR}/bot_token.txt" "${CMAKE_BINARY_DIR}/db_connection.txt"
     POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TOKEN_PATH} "${CMAKE_BINARY_DIR}/bot_token.txt"
     COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DB_CREDS_PATH} "${CMAKE_BINARY_DIR}/db_connection.txt"
 )
 
 add_custom_command(
	OUTPUT "${CMAKE_BINARY_DIR}/libdpp.so.1.0"
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DPP_BINARY_PATH} "${CMAKE_BINARY_DIR}/libdpp.so.1.0"
 )

 add_custom_target(
     copy_credentials ALL DEPENDS "${CMAKE_BINARY_DIR}/bot_token.txt" "${CMAKE_BINARY_DIR}/db_connection.txt"
 )

 add_custom_target(
     copy_dpp_library ALL DEPENDS "${CMAKE_BINARY_DIR}/libdpp.so.1.0" 
 )
 
 if(NOT GITHUB)
 	add_dependencies(${PROJECT_NAME} copy_credentials)
 	add_dependencies(${PROJECT_NAME} copy_dpp_library)
 endif()
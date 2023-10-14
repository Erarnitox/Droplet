cmake_minimum_required(VERSION 3.25)

include(cmake/utility.cmake)
include(cmake/CPM.cmake)

set(CPP_STANDARD 20)

project(cyber_drop 
	VERSION 1.0
	DESCRIPTION "A multipurpose Discord bot with the hacker in mind"
	HOMEPAGE_URL "https://cyberdrop.dropsoft.org"
	LANGUAGES CXX	
)

# disable in source builds
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(FATAL_ERROR "In source builds are not supported!")
endif()

option(USE_DEBUG_TOKEN "copy debug token and database connection")

# use interprocedural optimization
include(CheckIPOSupported)
check_ipo_Supported(RESULT ipo_supported)
if(ipo_supported)
	set(CMAKE_INTERPROCEDURAL_OPTIMITZATION True)
endif()

# disable compiler specific C++ extensions
set(CMAKE_CXX_EXTENSIONS OFF)

# download and update git submodules automatically
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
	message(STATUS "Updating Submodules...")
	execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
		RESULT_VARIABLE GIT_SUBMOD_RESULT)

	if(NOT GIT_SUBMOD_RESULT EQUAL "0")
    	message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
	endif()
else()
	message(FATAL_ERROR "git was not found!")
endif()

# out own source code for the project
add_subdirectory(src)

# subdirectory for third party dependencies
add_subdirectory(extern)

# treat the dpp include directory like a system/compiler include
target_include_directories(utility SYSTEM PUBLIC
	extern/DPP/include
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}
)

 set(TOKEN_PATH "${CMAKE_CURRENT_LIST_DIR}/src/bot_token.txt")
 set(DB_CREDS_PATH "${CMAKE_CURRENT_LIST_DIR}/src/db_connection.txt")
 set(DPP_BINARY_PATH "${CMAKE_BINARY_DIR}/extern/DPP/library/libdpp.so.1.0")
 set(BOT_BINARY_PATH "${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}")
 
 add_custom_command(
 	OUTPUT "${CMAKE_BINARY_DIR}/libdpp.so.1.0"
 	POST_BUILD
 	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DPP_BINARY_PATH} "${CMAKE_BINARY_DIR}/libdpp.so.1.0"
 )

 add_custom_target(
     copy_dpp_library ALL DEPENDS "${CMAKE_BINARY_DIR}/libdpp.so.1.0" 
 )

 if(USE_DEBUG_TOKEN)
 	add_custom_command(
    	OUTPUT "${CMAKE_BINARY_DIR}/bot_token.txt" "${CMAKE_BINARY_DIR}/db_connection.txt"
    	POST_BUILD
     	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TOKEN_PATH} "${CMAKE_BINARY_DIR}/bot_token.txt"
     	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DB_CREDS_PATH} "${CMAKE_BINARY_DIR}/db_connection.txt"
 	)

 	add_custom_target(
    	copy_credentials ALL DEPENDS "${CMAKE_BINARY_DIR}/bot_token.txt" "${CMAKE_BINARY_DIR}/db_connection.txt"
 	)

 	add_dependencies(copy_credentials ${PROJECT_NAME})
 endif()
 
 add_dependencies(copy_dpp_library ${PROJECT_NAME})
 
 # tests
 add_subdirectory(test)

 # documenatation
 add_subdirectory(doc)

 # install the bot and it's library dependency
 install(TARGETS ${PROJECT_NAME} dpp)
 
 # include to automatically pack/make an installer for release
 include(CPack)
 
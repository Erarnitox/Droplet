cmake_minimum_required(VERSION 3.25)

include(cmake/utility.cmake)
include(cmake/CPM.cmake)

set(CPP_STANDARD 20)

set(BUILD_SHARED_LIBS OFF)
# set(OPENSSL_USE_STATIC_LIBS ON) #no-shared
# set(OPENSSL_TEST OFF) #no-test

find_package(OpenSSL REQUIRED)

project(droplet 
	VERSION 1.0.2 # https://semver.org/
	DESCRIPTION "A multipurpose Discord bot with the hacker in mind"
	HOMEPAGE_URL "https://droplet.dropsoft.org"
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

# subdirectory for third party dependencies
add_subdirectory(extern)

# source code for the project
add_subdirectory(src bin)

set(TOKEN_PATH "${CMAKE_CURRENT_LIST_DIR}/src/creds/bot_token.txt")
set(DB_CREDS_PATH "${CMAKE_CURRENT_LIST_DIR}/src/creds/db_connection.txt")
set(BOT_BINARY_PATH "${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}")
 
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
  
# tests
add_subdirectory(test)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
 	# documenatation
 	generate_doxygen("${CMAKE_CURRENT_LIST_DIR}/src" "${CMAKE_CURRENT_LIST_DIR}/docs/doxygen") # create doxygen target
	add_dependencies(${PROJECT_NAME} doxygen)

 	# dependecy graph
 	# generate_dep_graph("${CMAKE_CURRENT_LIST_DIR}/docs/dep_graph.png")
endif()
 
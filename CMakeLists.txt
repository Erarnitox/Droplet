cmake_minimum_required(VERSION 4.0)

if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/cmake/CPM.cmake")
  file(DOWNLOAD
    "https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/cpm.cmake"
    "${CMAKE_CURRENT_LIST_DIR}/cmake/CPM.cmake"
    STATUS status
    SHOW_PROGRESS
  )
  list(GET status 0 status_code)
  if(NOT status_code EQUAL 0)
    message(FATAL_ERROR "Could not download CPM.cmake")
  endif()
endif()
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(cmake/utility.cmake)
include(cmake/CPM.cmake)

set(CPP_STANDARD 23)
set(CXX_STANDARD 23)

add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=1)
add_compile_definitions(GLIBCXX_USE_CXX11_ABI=1)

set(BUILD_SHARED_LIBS OFF)
# set(OPENSSL_USE_STATIC_LIBS ON) #no-shared
set(OPENSSL_TEST OFF) #no-test
set(DPP_BUILD_TEST OFF)

find_package(OpenSSL REQUIRED)

project(droplet 
	VERSION 1.3.0 # https://semver.org/
	DESCRIPTION "A multipurpose Discord bot with the hacker in mind"
	HOMEPAGE_URL "https://droplet.erarnitox.de"
	LANGUAGES C CXX
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
set(SECRETS_PATH "${CMAKE_CURRENT_LIST_DIR}/src/creds/secrets.json")
set(DB_CREDS_PATH "${CMAKE_CURRENT_LIST_DIR}/src/creds/db_connection.txt")
set(BOT_BINARY_PATH "${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}")
set(CERTIFICATE_FILE "${CMAKE_BINARY_DIR}/server.crt")
set(KEY_FILE "${CMAKE_BINARY_DIR}/server.key")

add_custom_target(generate_ssl_cert
    DEPENDS ${KEY_FILE} ${CERTIFICATE_FILE}
)
 
if(USE_DEBUG_TOKEN)
	add_custom_command(
    	OUTPUT "${CMAKE_BINARY_DIR}/bot_token.txt" "${CMAKE_BINARY_DIR}/db_connection.txt" "${CMAKE_BINARY_DIR}/secrets.json"
    	# POST_BUILD
     	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TOKEN_PATH} "${CMAKE_BINARY_DIR}/bot_token.txt"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SECRETS_PATH} "${CMAKE_BINARY_DIR}/secrets.json"
     	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DB_CREDS_PATH} "${CMAKE_BINARY_DIR}/db_connection.txt"
 	)

	add_custom_command(
		OUTPUT ${KEY_FILE} ${CERTIFICATE_FILE}
		COMMAND ${CMAKE_COMMAND} -E echo "Generating SSL key and certificate..."
		COMMAND openssl req -newkey rsa:2048 -nodes -keyout ${KEY_FILE} -x509 -days 365 -out ${CERTIFICATE_FILE} -subj "/CN=localhost"
		COMMENT "Generating self-signed SSL certificate and key"
		VERBATIM
	)

 	add_custom_target(
    	copy_credentials ALL DEPENDS "${CMAKE_BINARY_DIR}/bot_token.txt" "${CMAKE_BINARY_DIR}/db_connection.txt" "${CMAKE_BINARY_DIR}/secrets.json" ${KEY_FILE} ${CERTIFICATE_FILE}
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
 	generate_dep_graph()
	add_dependencies(dep_graph ${PROJECT_NAME})

	copy_compile_commands()
endif()
 
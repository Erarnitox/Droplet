message(STATUS "Updating Submodules...")
execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE GIT_SUBMOD_RESULT)
if(NOT GIT_SUBMOD_RESULT EQUAL "0")
    message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
endif()

set(BUILD_SHARED_LIBS ON)
add_subdirectory(DPP)

set_target_properties(dpp PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}
	BUILD_SHARED_LIBS OFF
	DPP_INSTALL OFF
	BUILD_VOICE_SUPPORT OFF
	DPP_BUILD_TEST OFF
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	dpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
	libs/DPP/include
)

add_subdirectory(bot_library)

add_executable(${PROJECT_NAME}
	main.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD ${CPP_STANDARD}
	CXX_STANDARD_REQUIRED ON
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

droplet_target_warnings(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE bot_library)

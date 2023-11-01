add_subdirectory(bot_library)

add_executable(${PROJECT_NAME}
	main.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)

target_link_libraries(${PROJECT_NAME} PRIVATE bot_library)

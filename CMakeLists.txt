cmake_minimum_required(VERSION 3.25)

project(cyber_drop VERSION 1.0)

add_subdirectory(src)

add_subdirectory(libs)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)

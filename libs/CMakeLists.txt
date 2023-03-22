set(BUILD_SHARED_LIBS ON)

add_subdirectory(DPP)

set_target_properties(dpp PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	dpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
	libs/DPP/include
)

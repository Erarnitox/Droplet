set(BUILD_SHARED_LIBS ON)

add_subdirectory(DPP)

target_link_libraries(${PROJECT_NAME} 
	dpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
	libs/DPP/include
)

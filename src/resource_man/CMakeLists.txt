add_library(resource_man STATIC
	resource_man.cpp
)

target_include_directories(resource_man PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE resource_man)

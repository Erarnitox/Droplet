add_subdirectory(database)
target_link_libraries(${PROJECT_NAME} PRIVATE database)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

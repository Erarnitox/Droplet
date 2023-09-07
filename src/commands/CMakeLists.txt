add_library(commands STATIC 
    CommandRegister.cpp
    CommandRegister.hpp
)

add_subdirectory(help)
add_subdirectory(resources)
add_subdirectory(roles)
add_subdirectory(users)
add_subdirectory(challenge_role)
add_subdirectory(set_channel)

target_link_libraries(commands PRIVATE core)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE commands) 
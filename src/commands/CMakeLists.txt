add_library(commands STATIC 
    Commands.cpp
    Commands.hpp
)

add_subdirectory(help)
add_subdirectory(resources)
add_subdirectory(roles)
add_subdirectory(users)
add_subdirectory(challenge_role)

target_link_libraries(${PROJECT_NAME} PRIVATE commands) 
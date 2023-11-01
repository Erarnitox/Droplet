add_library(commands STATIC 
    Commands.cpp
)

# add_subdirectory(help)
# add_subdirectory(resources)
# add_subdirectory(roles)
# add_subdirectory(users)
# add_subdirectory(challenge_role)
# add_subdirectory(set_channel)
add_subdirectory(pong)

target_link_libraries(commands PUBLIC bot)
target_include_directories(commands PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

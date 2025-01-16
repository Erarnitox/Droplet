add_library(commands STATIC 
    Commands.cpp
)

target_include_directories(commands PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

## Only needed when porting to a new database
# add_subdirectory(init_database)

add_subdirectory(help)
add_subdirectory(resources)
add_subdirectory(reaction_roles)
add_subdirectory(challenge_role)
add_subdirectory(clean_up)
add_subdirectory(server_details)
add_subdirectory(set_status)
add_subdirectory(invite)
add_subdirectory(roll)
add_subdirectory(flip)
add_subdirectory(challenge_badge)
add_subdirectory(profile)
add_subdirectory(portal)

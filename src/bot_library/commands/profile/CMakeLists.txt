target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(profile_command STATIC
    ProfileCommand.cpp
    ProfileCommand.hpp
)

droplet_target_warnings(profile_command)
set_property(TARGET profile_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(profile_command PUBLIC bot)
target_link_libraries(profile_command PUBLIC persistence)
target_link_libraries(commands PRIVATE profile_command)

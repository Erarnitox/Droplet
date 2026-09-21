target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(invite_command STATIC
    InviteCommand.cpp
    InviteCommand.hpp
)

droplet_target_warnings(invite_command)
set_property(TARGET invite_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(invite_command PUBLIC bot)
target_link_libraries(commands PRIVATE invite_command)

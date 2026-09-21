target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(roll_command STATIC
    RollCommand.cpp
    RollCommand.hpp
)

droplet_target_warnings(roll_command)
set_property(TARGET roll_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(roll_command PUBLIC bot)
target_link_libraries(roll_command PUBLIC persistence)
target_link_libraries(commands PRIVATE roll_command)

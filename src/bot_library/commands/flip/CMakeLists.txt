target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(flip_command STATIC
    FlipCommand.cpp
    FlipCommand.hpp
)

droplet_target_warnings(flip_command)
set_property(TARGET flip_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(flip_command PUBLIC bot)
target_link_libraries(flip_command PUBLIC persistence)
target_link_libraries(commands PRIVATE flip_command)

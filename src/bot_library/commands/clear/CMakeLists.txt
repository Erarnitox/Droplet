target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(clear_command STATIC
    ClearCommand.cpp
    ClearCommand.hpp
)

droplet_target_warnings(clear_command)
set_property(TARGET clear_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(clear_command PUBLIC bot)
target_link_libraries(commands PRIVATE clear_command)

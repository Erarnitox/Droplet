target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(help_command STATIC
    HelpCommand.cpp
    HelpCommand.hpp
)

droplet_target_warnings(help_command)
set_property(TARGET help_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(help_command PUBLIC bot)
target_link_libraries(commands PRIVATE help_command)

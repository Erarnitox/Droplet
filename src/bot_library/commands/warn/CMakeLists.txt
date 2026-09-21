target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(warn_command STATIC
    WarnCommand.cpp
    WarnCommand.hpp
)

droplet_target_warnings(warn_command)
set_property(TARGET warn_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(warn_command PUBLIC bot)
target_link_libraries(commands PRIVATE warn_command)

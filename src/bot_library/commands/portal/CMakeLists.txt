target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(portal_command STATIC
    SetPortalCommand.cpp
    SetPortalCommand.hpp
    RemovePortalCommand.cpp
    RemovePortalCommand.hpp
    BlacklistCommand.cpp
    BlacklistCommand.hpp
)

droplet_target_warnings(portal_command)
set_property(TARGET portal_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(portal_command PUBLIC bot)
target_link_libraries(portal_command PUBLIC persistence)
target_link_libraries(commands PRIVATE portal_command)

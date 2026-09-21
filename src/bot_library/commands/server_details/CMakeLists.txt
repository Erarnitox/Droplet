target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(server_details_command STATIC
    ServerDetails.cpp
    ServerDetails.hpp
)

droplet_target_warnings(server_details_command)
set_property(TARGET server_details_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(server_details_command PUBLIC bot)
target_link_libraries(commands PRIVATE server_details_command)

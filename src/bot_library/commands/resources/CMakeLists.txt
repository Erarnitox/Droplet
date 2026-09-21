target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(resources_command STATIC
    AddResourcesCommand.cpp
    AddResourcesCommand.hpp
    GetResourcesCommand.cpp
    GetResourcesCommand.hpp
)

droplet_target_warnings(resources_command)
set_property(TARGET resources_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(resources_command PUBLIC bot)
target_link_libraries(resources_command PUBLIC persistence)
target_link_libraries(commands PRIVATE resources_command)

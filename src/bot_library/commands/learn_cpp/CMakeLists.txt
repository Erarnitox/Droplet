target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(learncpp_command STATIC
    LearnCPPCommand.cpp
    LearnCPPCommand.hpp
    LearnCppTutorialHint.cpp
    LearnCppTutorialHint.hpp
)

droplet_target_warnings(learncpp_command)
set_property(TARGET learncpp_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(learncpp_command PUBLIC bot)
target_link_libraries(commands PRIVATE learncpp_command)

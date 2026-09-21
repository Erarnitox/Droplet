target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(set_status STATIC
    SetStatus.cpp
    SetStatus.hpp
)

droplet_target_warnings(set_status)
set_property(TARGET set_status PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(set_status PUBLIC bot)
target_link_libraries(commands PRIVATE set_status)

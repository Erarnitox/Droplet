target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(clean_up STATIC
    CleanUp.cpp
    CleanUp.hpp
)

droplet_target_warnings(clean_up)
set_property(TARGET clean_up PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(clean_up PUBLIC bot)
target_link_libraries(clean_up PUBLIC persistence)
target_link_libraries(commands PRIVATE clean_up)

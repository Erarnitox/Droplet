target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(reaction_roles STATIC
    ReactionRoles.cpp
    ReactionRoles.hpp
)

droplet_target_warnings(reaction_roles)
set_property(TARGET reaction_roles PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(reaction_roles PUBLIC bot)
target_link_libraries(reaction_roles PUBLIC persistence)
target_link_libraries(commands PRIVATE reaction_roles)

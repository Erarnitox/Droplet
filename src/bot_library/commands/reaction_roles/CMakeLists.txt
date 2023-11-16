target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(reaction_roles STATIC
    ReactionRoles.cpp
    ReactionRoles.hpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(reaction_roles PRIVATE "/W4")
else()
    target_compile_options(reaction_roles PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET reaction_roles PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(reaction_roles PUBLIC bot)

target_link_libraries(commands PRIVATE reaction_roles)

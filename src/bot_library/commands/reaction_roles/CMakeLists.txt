target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(reaction_roles STATIC
    ReactionRoles.cpp
    ReactionRoles.hpp
)

add_library(reaction_role_repository STATIC
    ReactionRoleRepository.cpp
    ReactionRoleRepository.hpp
    ReactionRoleDTO.hpp
    ReactionRoleDTO.cpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(reaction_roles PRIVATE "/W4")
    target_compile_options(reaction_role_repository PRIVATE "/W4")
else()
    target_compile_options(reaction_roles PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
    target_compile_options(reaction_role_repository PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET reaction_roles PROPERTY CXX_STANDARD ${CPP_STANDARD})
set_property(TARGET reaction_role_repository PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(reaction_role_repository PRIVATE database)
target_link_libraries(reaction_role_repository PRIVATE bot)

target_link_libraries(reaction_roles PRIVATE reaction_role_repository)
target_link_libraries(reaction_roles PUBLIC bot)

target_link_libraries(commands PRIVATE reaction_roles)

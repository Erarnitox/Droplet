add_library(commands STATIC 
    CommandRegister.cpp
    CommandRegister.hpp
)

add_subdirectory(help)
add_subdirectory(resources)
add_subdirectory(roles)
add_subdirectory(users)
add_subdirectory(challenge_role)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(challenge_role PRIVATE "/W4")
    target_compile_options(challenge_role_repository PRIVATE "/W4")
else()
    target_compile_options(challenge_role PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
    target_compile_options(challenge_role_repository PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET challenge_role PROPERTY CXX_STANDARD ${CPP_STANDARD})
set_property(TARGET challenge_role_repository PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(commands PRIVATE core)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE commands) 
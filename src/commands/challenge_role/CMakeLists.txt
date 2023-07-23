target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(challenge_role STATIC
    ChallengeRoleCommand.cpp
    ChallengeRoleCommand.hpp
)

add_library(challenge_role_repository STATIC
    ChallengeRoleRepository.cpp
    ChallengeRoleRepository.hpp
    ChallengeRoleDTO.hpp
)

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

target_link_libraries(challenge_role_repository PRIVATE database)
target_link_libraries(challenge_role_repository PUBLIC core)
target_link_libraries(challenge_role PRIVATE challenge_role_repository)
target_link_libraries(${PROJECT_NAME} PRIVATE challenge_role)
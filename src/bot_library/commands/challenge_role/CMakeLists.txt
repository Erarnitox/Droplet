target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(challenge_role STATIC
    ChallengeRoleCommand.cpp
    ChallengeRoleCommand.hpp
)

droplet_target_warnings(challenge_role)
set_property(TARGET challenge_role PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(challenge_role PUBLIC bot)
target_link_libraries(challenge_role PUBLIC persistence)
target_link_libraries(commands PRIVATE challenge_role)

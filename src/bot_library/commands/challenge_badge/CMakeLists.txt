target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(challenge_badge STATIC
    ChallengeBadgeCommand.cpp
    ChallengeBadgeCommand.hpp
)

droplet_target_warnings(challenge_badge)
set_property(TARGET challenge_badge PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(challenge_badge PUBLIC bot)
target_link_libraries(challenge_badge PUBLIC persistence)
target_link_libraries(commands PRIVATE challenge_badge)

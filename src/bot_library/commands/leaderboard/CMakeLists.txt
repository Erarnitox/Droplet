target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(leaderboard_command STATIC
    LeaderboardCommand.cpp
    LeaderboardCommand.hpp
)

droplet_target_warnings(leaderboard_command)
set_property(TARGET leaderboard_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(leaderboard_command PUBLIC bot)
target_link_libraries(leaderboard_command PUBLIC persistence)
target_link_libraries(commands PRIVATE leaderboard_command)

target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR}/repositories)

add_library(leaderboard_command STATIC
    LeaderboardCommand.cpp
    LeaderboardCommand.hpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(leaderboard_command PRIVATE "/W4")
else()
    target_compile_options(leaderboard_command PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET leaderboard_command PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(leaderboard_command PUBLIC bot)
target_link_libraries(leaderboard_command PRIVATE user_repository)

target_link_libraries(commands PRIVATE leaderboard_command)

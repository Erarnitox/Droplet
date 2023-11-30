target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(profile_command STATIC
    ProfileCommand.cpp
    ProfileCommand.hpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(profile_command PRIVATE "/W4")
else()
    target_compile_options(profile_command PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET profile_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(profile_command PUBLIC bot)

target_link_libraries(profile_command PRIVATE user_repository)
target_link_libraries(profile_command PRIVATE challenge_badge_repository)
target_link_libraries(profile_command PRIVATE has_badge_repository)

target_link_libraries(commands PRIVATE profile_command)

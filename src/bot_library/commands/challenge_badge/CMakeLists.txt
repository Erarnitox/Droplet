target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(challenge_badge STATIC
    ChallengeBadgeCommand.cpp
    ChallengeBadgeCommand.hpp
)

add_library(challenge_badge_repository STATIC
    ChallengeBadgeRepository.cpp
    ChallengeBadgeRepository.hpp
    ChallengeBadgeDTO.hpp
    ChallengeBadgeDTO.cpp
)

add_library(user_repository STATIC
    UserRepository.cpp
    UserRepository.hpp
    UserDTO.hpp
    UserDTO.cpp
)

add_library(has_badge_repository STATIC
    HasBadgeRepository.cpp
    HasBadgeRepository.hpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(challenge_badge PRIVATE "/W4")
    target_compile_options(challenge_badge_repository PRIVATE "/W4")
    target_compile_options(user_repository PRIVATE "/W4")
    target_compile_options(has_badge_repository PRIVATE "/W4")
else()
    target_compile_options(challenge_badge PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
    target_compile_options(challenge_badge_repository PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
    target_compile_options(user_repository PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
    target_compile_options(has_badge_repository PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

target_include_directories(challenge_badge_repository PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(user_repository PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(has_badge_repository PUBLIC ${CMAKE_CURRENT_LIST_DIR})

set_property(TARGET challenge_badge PROPERTY CXX_STANDARD ${CPP_STANDARD})
set_property(TARGET challenge_badge_repository PROPERTY CXX_STANDARD ${CPP_STANDARD})
set_property(TARGET user_repository PROPERTY CXX_STANDARD ${CPP_STANDARD})
set_property(TARGET has_badge_repository PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(challenge_badge_repository PRIVATE database)
target_link_libraries(challenge_badge_repository PRIVATE bot)

target_link_libraries(user_repository PRIVATE database)
target_link_libraries(user_repository PRIVATE bot)

target_link_libraries(has_badge_repository PRIVATE database)
target_link_libraries(has_badge_repository PRIVATE bot)

target_link_libraries(challenge_badge PRIVATE user_repository)
target_link_libraries(challenge_badge PRIVATE challenge_badge_repository)
target_link_libraries(challenge_badge PRIVATE has_badge_repository)
target_link_libraries(challenge_badge PUBLIC bot)

target_link_libraries(commands PRIVATE challenge_badge)

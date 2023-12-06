target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(roll_command STATIC
    RollCommand.cpp
    RollCommand.hpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(roll_command PRIVATE "/W4")
else()
    target_compile_options(roll_command PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET roll_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(roll_command PUBLIC bot)
target_link_libraries(roll_command PRIVATE user_repository)

target_link_libraries(commands PRIVATE roll_command)

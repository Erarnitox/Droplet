target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(reminder_command STATIC
    ReminderCommand.cpp
    ReminderCommand.hpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(reminder_command PRIVATE "/W4")
else()
    target_compile_options(reminder_command PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET reminder_command PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(reminder_command PUBLIC bot)
target_link_libraries(commands PRIVATE reminder_command)

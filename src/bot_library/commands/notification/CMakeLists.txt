target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(notification_command STATIC
    SetNotificationCommand.cpp
    SetNotificationCommand.hpp
    RemoveNotificationCommand.cpp
    RemoveNotificationCommand.hpp
)

add_subdirectory(repositories)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(notification_command PRIVATE "/W4")
else()
    target_compile_options(notification_command PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET notification_command PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(notification_command PUBLIC bot)
target_link_libraries(commands PRIVATE notification_command)

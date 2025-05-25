target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(bug_report_command STATIC
    BugReportCommand.cpp
    BugReportCommand.hpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(bug_report_command PRIVATE "/W4")
else()
    target_compile_options(bug_report_command PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET bug_report_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(bug_report_command PUBLIC bot)

target_link_libraries(commands PRIVATE bug_report_command)

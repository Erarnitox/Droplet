target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(bug_report_command STATIC
    BugReportCommand.cpp
    BugReportCommand.hpp
)

droplet_target_warnings(bug_report_command)
set_property(TARGET bug_report_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(bug_report_command PUBLIC bot)
target_link_libraries(commands PRIVATE bug_report_command)

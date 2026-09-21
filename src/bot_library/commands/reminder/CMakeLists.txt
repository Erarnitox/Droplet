target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(reminder_command STATIC
    ReminderCommand.cpp
    ReminderCommand.hpp
)

droplet_target_warnings(reminder_command)
set_property(TARGET reminder_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(reminder_command PUBLIC bot)
target_link_libraries(commands PRIVATE reminder_command)

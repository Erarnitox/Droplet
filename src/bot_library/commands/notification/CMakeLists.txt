target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(notification_command STATIC
    SetNotificationCommand.cpp
    SetNotificationCommand.hpp
    RemoveNotificationCommand.cpp
    RemoveNotificationCommand.hpp
    YoutubeFeed.cpp
    YoutubeFeed.hpp
    YoutubeNotificationService.cpp
    YoutubeNotificationService.hpp
)

droplet_target_warnings(notification_command)
set_property(TARGET notification_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(notification_command PUBLIC bot)
target_link_libraries(notification_command PUBLIC persistence)

CPMAddPackage("gh:libcpr/cpr#1.11.1")
target_link_libraries_system(notification_command PUBLIC cpr::cpr)

target_link_libraries(commands PRIVATE notification_command)

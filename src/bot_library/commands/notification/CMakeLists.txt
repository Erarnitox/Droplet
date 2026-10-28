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

# cpr >= 1.13 builds libpsl (needs meson) for its bundled curl unless told not to.
# The curl used here is the one declared in bot/email (8.23.0, libpsl OFF, SMTP/HTTP
# client only), so keep libpsl off here too: no Public Suffix List, no meson build dep.
CPMAddPackage(
    NAME cpr
    GITHUB_REPOSITORY libcpr/cpr
    GIT_TAG 1.15.1
    OPTIONS
        "CPR_CURL_USE_LIBPSL OFF"
)
target_link_libraries_system(notification_command PUBLIC cpr::cpr)

target_link_libraries(commands PRIVATE notification_command)

add_subdirectory(core)
add_subdirectory(secrets)
add_subdirectory(email)

add_library(bot STATIC
    Bot.cpp
    BotSlashCommandRegistrar.cpp
    BotEventSubscriptions.cpp
)

droplet_target_warnings(bot)
set_property(TARGET bot PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_include_directories(bot PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
target_link_libraries(bot PUBLIC core)
target_link_libraries(bot PUBLIC secrets)
target_link_libraries(bot PUBLIC email)

add_library(welcome_channel_repository STATIC
    WelcomeChannelRepository.cpp
    WelcomeChannelRepository.hpp
    WelcomeChannelDTO.hpp
    WelcomeChannelDTO.cpp
)

add_library(goodbye_channel_repository STATIC
    GoodbyeChannelRepository.cpp
    GoodbyeChannelRepository.hpp
    GoodbyeChannelDTO.hpp
    GoodbyeChannelDTO.cpp
)

add_library(log_channel_repository STATIC
    LogChannelRepository.cpp
    LogChannelRepository.hpp
    LogChannelDTO.hpp
    LogChannelDTO.cpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(welcome_channel_repository PRIVATE "/W4")
    target_compile_options(goodbye_channel_repository PRIVATE "/W4")
    target_compile_options(log_channel_repository PRIVATE "/W4")
else()
    target_compile_options(welcome_channel_repository PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
    target_compile_options(goodbye_channel_repository PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
    target_compile_options(log_channel_repository PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET welcome_channel_repository PROPERTY CXX_STANDARD ${CPP_STANDARD})
set_property(TARGET goodbye_channel_repository PROPERTY CXX_STANDARD ${CPP_STANDARD})
set_property(TARGET log_channel_repository PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(welcome_channel_repository PRIVATE bot)
target_link_libraries(goodbye_channel_repository PRIVATE bot)
target_link_libraries(log_channel_repository PRIVATE bot)

target_include_directories(set_channel_command PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(set_channel_command PRIVATE welcome_channel_repository)
target_link_libraries(set_channel_command PRIVATE goodbye_channel_repository)
target_link_libraries(set_channel_command PRIVATE log_channel_repository)

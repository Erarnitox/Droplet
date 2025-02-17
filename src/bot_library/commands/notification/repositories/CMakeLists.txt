add_library(notification_repository STATIC
    NotificationRepository.cpp
    NotificationRepository.hpp
    NotificationDTO.hpp
    NotificationDTO.cpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(notification_repository PRIVATE "/W4")
else()
    target_compile_options(notification_repository PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET notification_repository PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(notification_repository PRIVATE bot)

target_include_directories(notification_repository PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(notification_command PRIVATE notification_repository)

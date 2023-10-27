target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(pong_command STATIC
    PongCommand.cpp
    PongCommand.hpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(pong_command PRIVATE "/W4")
else()
    target_compile_options(pong_command PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET pong_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(pong_command PUBLIC core)

target_link_libraries(commands PRIVATE pong_command)
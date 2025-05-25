target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(warn_command STATIC
    WarnCommand.cpp
    WarnCommand.hpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(warn_command PRIVATE "/W4")
else()
    target_compile_options(warn_command PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET warn_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(warn_command PUBLIC bot)

target_link_libraries(commands PRIVATE warn_command)

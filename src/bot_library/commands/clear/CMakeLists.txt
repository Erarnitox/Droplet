target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(clear_command STATIC
    ClearCommand.cpp
    ClearCommand.hpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(clear_command PRIVATE "/W4")
else()
    target_compile_options(clear_command PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET clear_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(clear_command PUBLIC bot)

target_link_libraries(commands PRIVATE clear_command)

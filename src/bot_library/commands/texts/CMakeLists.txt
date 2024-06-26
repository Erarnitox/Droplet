target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(texts_command STATIC
    TextsCommand.cpp
    TextsCommand.hpp
)

add_subdirectory(repositories)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(texts_command PRIVATE "/W4")
else()
    target_compile_options(texts_command PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET texts_command PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(texts_command PUBLIC bot)
target_link_libraries(commands PRIVATE texts_command)

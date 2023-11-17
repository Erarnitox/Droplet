target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(resources_command STATIC
    ResourcesCommand.cpp
    ResourcesCommand.hpp
)

add_subdirectory(repositories)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(resources_command PRIVATE "/W4")
else()
    target_compile_options(resources_command PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET resources_command PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(resources_command PUBLIC bot)
target_link_libraries(commands PRIVATE resources_command)

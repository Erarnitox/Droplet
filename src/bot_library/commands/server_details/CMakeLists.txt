target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(server_details_command STATIC
    ServerDetails.cpp
    ServerDetails.hpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(server_details_command PRIVATE "/W4")
else()
    target_compile_options(server_details_command PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET server_details_command PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(server_details_command PUBLIC bot)

target_link_libraries(commands PRIVATE server_details_command)

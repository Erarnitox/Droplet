target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(set_status STATIC
    SetStatus.cpp
    SetStatus.hpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(set_status PRIVATE "/W4")
else()
    target_compile_options(set_status PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET set_status PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(set_status PUBLIC bot)

target_link_libraries(commands PRIVATE set_status)

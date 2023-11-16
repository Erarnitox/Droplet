target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(init_database STATIC
    InitDatabase.cpp
    InitDatabase.hpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(init_database PRIVATE "/W4")
else()
    target_compile_options(init_database PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET init_database PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(init_database PUBLIC bot)

target_link_libraries(commands PRIVATE init_database)

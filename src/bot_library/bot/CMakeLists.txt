add_subdirectory(core)

add_library(bot STATIC
    Bot.cpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(bot PRIVATE "/W4")
else()
    target_compile_options(bot PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET bot PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_include_directories(bot PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

target_link_libraries(bot PUBLIC core)


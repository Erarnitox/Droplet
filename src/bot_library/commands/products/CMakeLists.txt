target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(products_command STATIC
    ProductsCommand.cpp
    ProductsCommand.hpp
)

add_subdirectory(repositories)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(products_command PRIVATE "/W4")
else()
    target_compile_options(products_command PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET products_command PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(products_command PUBLIC bot)
target_link_libraries(commands PRIVATE products_command)

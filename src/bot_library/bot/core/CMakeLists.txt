add_subdirectory(database)

add_library(core STATIC 
    Core.cpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(core PRIVATE "/W4")
else()
    target_compile_options(core PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

target_include_directories(core PUBLIC "${CMAKE_CURRENT_LIST_DIR}/interfaces/include")
target_include_directories(core PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

set_property(TARGET core PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries_system(core dpp)

target_link_libraries(core PUBLIC database)

add_subdirectory(database)
add_subdirectory(utility)

add_library(core STATIC 
    Core.cpp
    Core.hpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(core PRIVATE "/W4")
else()
    target_compile_options(core PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET core PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(core PUBLIC database)
target_link_libraries(core PUBLIC utility)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_LIST_DIR}/interfaces)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_LIST_DIR}/utility)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(core PUBLIC 
	fmt::fmt
)

target_link_libraries(${PROJECT_NAME} PRIVATE core)
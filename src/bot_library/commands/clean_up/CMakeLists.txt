target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(clean_up STATIC
    CleanUp.cpp
    CleanUp.hpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(clean_up PRIVATE "/W4")
else()
    target_compile_options(clean_up PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET clean_up PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(clean_up PRIVATE reaction_role_repository)
target_link_libraries(clean_up PRIVATE challenge_role_repository)

target_link_libraries(clean_up PUBLIC bot)

target_link_libraries(commands PRIVATE clean_up)

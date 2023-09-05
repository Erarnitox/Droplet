add_library(utility STATIC 
    Commands.cpp
    Commands.hpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    #target_compile_options(utility PRIVATE "/W4")
else()
    #target_compile_options(utility PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

target_link_libraries(core PRIVATE
    dpp
)

set_property(TARGET utility PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_include_directories(utility PUBLIC ${CMAKE_CURRENT_LIST_DIR})
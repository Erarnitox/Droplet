add_library(secrets STATIC 
    Secrets.cpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    target_compile_options(secrets PRIVATE "/W4")
else()
    target_compile_options(secrets PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET secrets PROPERTY CXX_STANDARD 20)
target_include_directories(secrets PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

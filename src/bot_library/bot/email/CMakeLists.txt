add_library(email STATIC 
    EmailClient.cpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    target_compile_options(email PRIVATE "/W4")
else()
    target_compile_options(email PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET email PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_include_directories(email PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

target_link_libraries(email PRIVATE core)
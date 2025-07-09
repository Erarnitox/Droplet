CPMAddPackage("gh:jtv/libpqxx#7.10.1")

add_library(database STATIC 
    Database.cpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    target_compile_options(database PRIVATE "/W4")
else()
    target_compile_options(database PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET database PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_include_directories(database PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

target_link_libraries(database PUBLIC pqxx)

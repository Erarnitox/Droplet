CPMAddPackage("gh:jtv/libpqxx#8.0.2")

add_library(database STATIC
    ConnectionPool.cpp
)

droplet_target_warnings(database)
set_property(TARGET database PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_include_directories(database PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
target_link_libraries_system(database PUBLIC pqxx)

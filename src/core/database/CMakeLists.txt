add_library(database STATIC 
    Database.cpp
    Database.hpp
)

set_property(TARGET database PROPERTY CXX_STANDARD 20)
target_include_directories(database PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(database PUBLIC pqxx fmt::fmt)
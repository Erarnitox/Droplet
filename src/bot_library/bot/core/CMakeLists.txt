add_subdirectory(database)

add_library(core STATIC
    Core.cpp
)

droplet_target_warnings(core)
target_include_directories(core PUBLIC "${CMAKE_CURRENT_LIST_DIR}/interfaces/include")
target_include_directories(core PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
set_property(TARGET core PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries_system(core PUBLIC dpp)
target_link_libraries(core PUBLIC database)

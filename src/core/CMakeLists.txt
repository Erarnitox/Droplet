add_subdirectory(database)

add_library(core STATIC 
    Core.cpp
    Core.hpp
)

set_property(TARGET core PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(core PUBLIC database)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_LIST_DIR}/interfaces)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(core PUBLIC 
	fmt::fmt
)

target_link_libraries(${PROJECT_NAME} PUBLIC core)
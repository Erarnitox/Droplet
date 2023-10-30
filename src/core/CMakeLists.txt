add_subdirectory(database)

add_library(core OBJECT 
    Core.cpp
    Core.hpp
)

add_library(bot OBJECT 
    Bot.hpp
    Bot.cpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(core PRIVATE "/W4")
    target_compile_options(bot PRIVATE "/W4")
else()
    target_compile_options(core PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
    target_compile_options(bot PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

target_include_directories(bot PUBLIC ${CMAKE_CURRENT_LIST_DIR}/interfaces)

set_property(TARGET core PROPERTY CXX_STANDARD ${CPP_STANDARD})

set_property(TARGET bot PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_link_libraries(bot PUBLIC ${DPP_LIBRARIES})
target_include_directories(bot PUBLIC ${DPP_INCLUDE_DIR})

target_link_libraries(core PUBLIC bot)
target_link_libraries(core PUBLIC database)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_LIST_DIR}/utility)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(core PUBLIC 
	fmt::fmt
)

target_link_libraries(${PROJECT_NAME} PUBLIC core)

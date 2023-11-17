add_library(resource_repository STATIC
    ResourceRepository.cpp
    ResourceRepository.hpp
    ResourceDTO.hpp
    ResourceDTO.cpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(resource_repository PRIVATE "/W4")
else()
    target_compile_options(resource_repository PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET resource_repository PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(resource_repository PRIVATE bot)

target_include_directories(resource_repository PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(resources_command PRIVATE resource_repository)

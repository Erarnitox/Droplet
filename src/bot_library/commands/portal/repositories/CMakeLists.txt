add_library(portal_repository STATIC
    PortalRepository.cpp
    PortalRepository.hpp
    PortalDTO.hpp
    PortalDTO.cpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(portal_repository PRIVATE "/W4")
else()
    target_compile_options(portal_repository PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET portal_repository PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(portal_repository PRIVATE bot)

target_include_directories(portal_repository PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(portal_command PRIVATE portal_repository)

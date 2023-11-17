add_library(member_repository STATIC
    MemberRepository.cpp
    MemberRepository.hpp
    MemberDTO.hpp
    MemberDTO.cpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(member_repository PRIVATE "/W4")
else()
    target_compile_options(member_repository PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET member_repository PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(member_repository PRIVATE bot)

target_include_directories(member_repository PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(member_command PRIVATE member_repository)

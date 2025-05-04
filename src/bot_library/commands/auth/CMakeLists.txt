target_include_directories(commands PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(web_user_repository STATIC
    WebUserRepository.cpp
    WebUserRepository.hpp
    WebUserDTO.hpp
    WebUserDTO.cpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(web_user_repository PRIVATE "/W4")
else()
    target_compile_options(web_user_repository PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

target_include_directories(web_user_repository PUBLIC ${CMAKE_CURRENT_LIST_DIR})

set_property(TARGET web_user_repository PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(web_user_repository PRIVATE database)
target_link_libraries(web_user_repository PRIVATE bot)

target_link_libraries(commands PUBLIC web_user_repository)

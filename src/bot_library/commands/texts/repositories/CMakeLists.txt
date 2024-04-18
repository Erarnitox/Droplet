add_library(text_repository STATIC
    TextRepository.cpp
    TextRepository.hpp
    TextDTO.hpp
    TextDTO.cpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(text_repository PRIVATE "/W4")
else()
    target_compile_options(text_repository PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET text_repository PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(text_repository PRIVATE bot)

target_include_directories(text_repository PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(texts_command PRIVATE text_repository)

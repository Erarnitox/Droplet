add_library(product_repository STATIC
    ProductRepository.cpp
    ProductRepository.hpp
    ProductDTO.hpp
    ProductDTO.cpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(product_repository PRIVATE "/W4")
else()
    target_compile_options(product_repository PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET product_repository PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries(product_repository PRIVATE bot)

target_include_directories(product_repository PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(products_command PRIVATE product_repository)

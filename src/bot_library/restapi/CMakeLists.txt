add_library(restapi STATIC
    RestApi.cpp
    JWTUtils.cpp
    AuthHandler.cpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(restapi PRIVATE "/W4")
else()
    target_compile_options(restapi PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

set_property(TARGET restapi PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_include_directories(restapi PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
target_link_libraries(restapi PRIVATE core)
target_link_libraries(restapi PRIVATE secrets)
target_link_libraries(restapi PRIVATE commands)

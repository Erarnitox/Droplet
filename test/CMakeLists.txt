# build/link static libraries
set(BUILD_SHARED_LIBS OFF)

# this variable is needed to load shared libraries from the binary dir
set(ENV(LD_LIBRARY_PATH) ${CMAKE_BINARY_DIR})

# make catch2 available
CPMAddPackage("gh:catchorg/Catch2@3.4.0")

# use CDash https://my.cdash.org
# (test dashboard for the build server maybe)
# (or just use jenkins)

# lcov can be used to view coverage information
# - https://github.com/linux-test-project/lcov

include(CTest)
# enable_testing() is automatically called

add_executable(unit_tests
    main_test.cpp
)

target_link_libraries(unit_tests PRIVATE ${PROJECT_NAME} Catch2::Catch2WithMain)
target_compile_features(unit_tests PRIVATE cxx_std_17)

# manually adding unittest
# add_test(NAME main_test COMMAND unit_tests)

# automatically discover all tests and make them known to CTest
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(unit_tests)

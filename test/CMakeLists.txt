# make catch2 available
CPMAddPackage("gh:catchorg/Catch2@3.8.1")

# use CDash https://my.cdash.org
# (test dashboard for the build server maybe)
# (or just use jenkins)

# lcov can be used to view coverage information
# - https://github.com/linux-test-project/lcov

include(CTest)
# enable_testing() is automatically called

add_executable(unit_tests
    main_test.cpp
    core_test.cpp
    database_test.cpp
)

target_link_libraries(unit_tests PRIVATE bot_library)
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)
target_compile_features(unit_tests PRIVATE cxx_std_17)

# manually adding unittest
# add_test(NAME main_test COMMAND unit_tests)

# automatically discover all tests and make them known to CTest
list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras")
include(Catch)
catch_discover_tests(unit_tests)

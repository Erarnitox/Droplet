# build/link static libraries
set(BUILD_SHARED_LIBS OFF)

# make catch2 available
CPMAddPackage("gh:catchorg/Catch2@3.4.0")

# output errors to the terminal when a test fails
# set(ENV{CTEST_OUTPUT_ON_FAILURE} ON)

# add_executable(unit_tests
#    unit_tests.cpp
#)

#target_include_directories(unit_tests PRIVATE ../src)

#add_test(NAME UnitTests COMMAND unit_tests)

# use CDash https://my.cdash.org
# (test dashboard for the build server maybe)
# (or just use jenkins)

#add_test(NAME testname
#    COMMAND ls -al)

#NOTE: lcov can be used to view coverage information
# - https://github.com/linux-test-project/lcov


add_executable(unit_tests
    main_test.cpp
)

target_link_libraries(unit_tests PRIVATE ${PROJECT_NAME} Catch2::Catch2WithMain)
target_compile_features(unit_tests PRIVATE cxx_std_17)

# Run tests on the build server automatically
# if(CMAKE_BUILD_TYPE STREQUAL Release)
    include(CTest)
    # enable_testing()

    # automatically discover all tests and make them known to CTest
    list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
    include(Catch)
    catch_discover_tests(unit_tests)
# endif()

include(CTest)
enable_testing()

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
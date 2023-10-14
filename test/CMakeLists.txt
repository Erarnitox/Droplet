include(CTest)

# output errors to the terminal when a test fails
set(ENV{CTEST_OUTPUT_ON_FAILURE} ON)

# use CDash https://my.cdash.org
# (test dashboard for the build server maybe)
# (or just use jenkins)

#add_test(NAME testname
#    COMMAND ls -al)
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
    youtube_feed_test.cpp
    auth_test.cpp
)

# bot_library already pulls notification_command via commands; linking it again
# duplicates ThinLTO bitcode and breaks ASan+LTO ("at most one ThinLTO module").
target_link_libraries(unit_tests PRIVATE bot_library)
target_link_libraries_system(unit_tests PRIVATE Catch2::Catch2WithMain)
target_compile_features(unit_tests PRIVATE cxx_std_23)
droplet_target_warnings(unit_tests)
# Catch2 TEST_CASE macros use __COUNTER__, which Clang pedantic treats as a C2y extension.
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_compile_options(unit_tests PRIVATE -Wno-c2y-extensions)
endif()

add_executable(hotpath_bench hotpath_bench.cpp)
target_compile_features(hotpath_bench PRIVATE cxx_std_23)
target_include_directories(hotpath_bench PRIVATE
	"${PROJECT_SOURCE_DIR}/src/bot_library/commands/learn_cpp")
droplet_target_warnings(hotpath_bench)

# manually adding unittest
# add_test(NAME main_test COMMAND unit_tests)

# automatically discover all tests and make them known to CTest
list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras")
include(Catch)
catch_discover_tests(unit_tests)

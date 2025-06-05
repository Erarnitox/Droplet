add_subdirectory(database)

add_library(core STATIC 
    Core.cpp
)

# enable additional warnings and static analysis for this target
if (MSVC)
    # for windows msvc or clang-cl
    target_compile_options(core PRIVATE "/W4")
else()
    target_compile_options(core PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

target_include_directories(core PUBLIC "${CMAKE_CURRENT_LIST_DIR}/interfaces/include")
target_include_directories(core PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

set_property(TARGET core PROPERTY CXX_STANDARD ${CPP_STANDARD})

target_link_libraries_system(core dpp)

target_link_libraries(core PUBLIC database)

find_package(OpenSSL REQUIRED)
CPMAddPackage(
    NAME curl
    GITHUB_REPOSITORY curl/curl
    GIT_TAG curl-8_10_1
    OPTIONS
        "CURL_USE_OPENSSL ON"
        "CMAKE_USE_LIBSSH2 OFF"
        "BUILD_CURL_EXE OFF"
        "CURL_ZLIB OFF"
)

CPMAddPackage("gh:libcpr/cpr#1.11.1")

if(cpr_ADDED)
  message(STATUS "CPR library added successfully.")
endif()

target_link_libraries(core PUBLIC CURL::libcurl)
target_link_libraries(core PUBLIC cpr::cpr)

# CPMAddPackage(
#  NAME Boost
#  VERSION 1.87.0 # Versions less than 1.85.0 may need patches for installation targets.
#  URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.tar.xz
#  URL_HASH SHA256=2c5ec5edcdff47ff55e27ed9560b0a0b94b07bd07ed9928b476150e16b0efc57
#  OPTIONS "BOOST_ENABLE_CMAKE ON" "BOOST_SKIP_INSTALL_RULES ON" # Set `OFF` for installation
#          "BUILD_SHARED_LIBS OFF" "BOOST_INCLUDE_LIBRARIES container\\\;asio" # Note the escapes!
#)
# target_include_directories(core PUBLIC ${Boost_INCLUDE_DIRS})

set("POCO_ENABLE_NETSSL" ON)
set("POCO_ENABLE_CRYPTO" ON)

CPMAddPackage("gh:pocoproject/poco#poco-1.14.1-release")

target_link_libraries(core PUBLIC
    Poco::Foundation
    Poco::Net
    Poco::NetSSL
    Poco::Util
    Poco::JSON
    Poco::JWT
    Poco::Crypto
)

target_include_directories(core PUBLIC ${Poco_SOURCE_DIR}/NetSSL/include)
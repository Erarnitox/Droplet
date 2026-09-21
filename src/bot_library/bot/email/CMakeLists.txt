add_library(email STATIC
    EmailClient.cpp
)

droplet_target_warnings(email)
set_property(TARGET email PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_include_directories(email PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
target_link_libraries(email PRIVATE core)

find_package(OpenSSL REQUIRED)
CPMAddPackage(
    NAME curl
    GITHUB_REPOSITORY curl/curl
    GIT_TAG curl-8_22_0
    OPTIONS
        "CURL_USE_OPENSSL ON"
        "CURL_USE_LIBSSH2 OFF"
        "BUILD_CURL_EXE OFF"
        "CURL_ZLIB OFF"
        "BUILD_TESTING OFF"
        "BUILD_LIBCURL_DOCS OFF"
        "ENABLE_CURL_MANUAL OFF"
)
target_link_libraries_system(email PUBLIC CURL::libcurl)

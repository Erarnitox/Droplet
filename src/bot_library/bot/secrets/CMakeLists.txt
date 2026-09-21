add_library(secrets STATIC
    Secrets.cpp
)

droplet_target_warnings(secrets)
set_property(TARGET secrets PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_include_directories(secrets PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
target_link_libraries(secrets PUBLIC core)

set("POCO_ENABLE_NETSSL" ON)
set("POCO_ENABLE_CRYPTO" ON)
CPMAddPackage("gh:pocoproject/poco#poco-1.15.3-release")
target_link_libraries_system(secrets PUBLIC
    Poco::Foundation
    Poco::Net
    Poco::NetSSL
    Poco::Util
    Poco::JSON
    Poco::JWT
    Poco::Crypto
)
target_include_directories(secrets SYSTEM PUBLIC ${Poco_SOURCE_DIR}/NetSSL/include)

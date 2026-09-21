add_library(restapi STATIC
    RestApi.cpp
    StatusHandler.cpp
    RestApiRequestHandlerFactory.cpp
    RestApiServerApplication.cpp
    JWTUtils.cpp
    AuthUtils.cpp
    AuthHandler.cpp
    UserManager.cpp
    NotFoundHandler.cpp
    RegistrationHandler.cpp
    VerifyHandler.cpp
    ResQueryHandler.cpp
    LeaderboardHandler.cpp
)

droplet_target_warnings(restapi)
set_property(TARGET restapi PROPERTY CXX_STANDARD ${CPP_STANDARD})
target_include_directories(restapi PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
target_link_libraries(restapi PRIVATE core)
target_link_libraries(restapi PRIVATE persistence)
target_link_libraries(restapi PUBLIC secrets)
target_link_libraries(restapi PRIVATE email)
find_package(OpenSSL REQUIRED)
target_link_libraries(restapi PRIVATE OpenSSL::Crypto)

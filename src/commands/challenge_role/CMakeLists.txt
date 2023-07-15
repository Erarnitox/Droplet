target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(challenge_role STATIC
    ChallengeRoleCommand.cpp
    ChallengeRoleCommand.hpp
)

add_library(challenge_role_repository STATIC
    ChallengeRoleRepository.cpp
    ChallengeRoleRepository.hpp
    ChallengeRoleDTO.hpp
)

target_link_libraries(challenge_role_repository PRIVATE database)
target_link_libraries(challenge_role_repository PUBLIC core)
target_link_libraries(challenge_role PRIVATE challenge_role_repository)
target_link_libraries(${PROJECT_NAME} PRIVATE challenge_role)
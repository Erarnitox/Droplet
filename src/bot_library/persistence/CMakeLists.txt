add_library(persistence STATIC
    PortalIndex.cpp
    PortalDTO.cpp
    PortalRepository.cpp
    BlacklistDTO.cpp
    BlacklistRepository.cpp
    ResourceDTO.cpp
    ResourceRepository.cpp
    NotificationDTO.cpp
    NotificationRepository.cpp
    LatestEventsRepository.cpp
    ReactionRoleDTO.cpp
    ReactionRoleRepository.cpp
    ChallengeRoleDTO.cpp
    ChallengeRoleRepository.cpp
    ChallengeBadgeDTO.cpp
    ChallengeBadgeRepository.cpp
    UserDTO.cpp
    UserRepository.cpp
    HasBadgeRepository.cpp
    WebUserDTO.cpp
    WebUserRepository.cpp
)

set_property(TARGET persistence PROPERTY CXX_STANDARD ${CPP_STANDARD})
droplet_target_warnings(persistence)
target_include_directories(persistence PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(persistence PUBLIC database)
target_link_libraries(persistence PUBLIC core)

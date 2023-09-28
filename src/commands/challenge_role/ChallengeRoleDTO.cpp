#include "ChallengeRoleCommand.hpp"
#include "ChallengeRoleDTO.hpp"

ChallengeRoleDTO::ChallengeRoleDTO(size_t roleId, size_t guildId, size_t messageId, const std::string& solution)
    : roleId{roleId}, guildId{guildId}, messageId{messageId}, solution{solution}{}

ChallengeRoleDTO::ChallengeRoleDTO(const RowDTOAdapter& row) {
    (void) row;
}
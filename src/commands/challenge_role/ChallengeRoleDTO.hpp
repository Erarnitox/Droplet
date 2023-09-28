#pragma once

#include <string>
#include <RowDTOAdapter.hpp>

class ChallengeRoleDTO {
    public:
        size_t roleId;
        size_t guildId; 
        size_t messageId;
        std::string solution;

        ChallengeRoleDTO() = default;
        ChallengeRoleDTO(size_t roleId, size_t guildId, size_t messageId, const std::string& solution);
        ChallengeRoleDTO(const RowDTOAdapter& row);
};
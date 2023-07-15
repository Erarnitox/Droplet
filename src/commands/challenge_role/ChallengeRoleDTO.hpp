#pragma once

#include <string>

class ChallengeRoleDTO {
    public:
        size_t roleId;
        size_t guildId; 
        size_t messageId;
        std::string solution;
};
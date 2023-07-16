#include "ChallengeRoleRepository.hpp"

#include <Database.hpp>

#include <cstddef>
#include <vector>

void ChallengeRoleRepository::create(const ChallengeRoleDTO& object) {
    static std::string sql_string{ "INSERT INTO challenge_roles(role_id, guild_id, message_id, flag) VALUES ($1, $2, $3, $4)" };
    
    if(!Database::hasConnection()) return;
    if(!object.messageId) return;
    if(this->get(object.messageId).guildId != 0) return;

    database::execQuery<4>(sql_string, object.roleId, object.guildId, object.messageId, object.solution);
}

void ChallengeRoleRepository::remove(size_t messageId) {
    static std::string sql_string{ "DELETE FROM challenge_roles WHERE id = $1" };
    
    if(!Database::hasConnection()) return;

    database::execQuery<1>(sql_string, messageId);
}
        
void ChallengeRoleRepository::update(size_t messageId, const ChallengeRoleDTO& object) {
    static std::string sql_string{ "UPDATE challenge_roles(role_id, guild_id, message_id, flag) VALUES ($1, $2, $3, $4) WHERE message_id = $3" };
    
    if(!Database::hasConnection()) return;
    if(!object.messageId) return;
    if(this->get(object.messageId).guildId != 0) return;

    database::execQuery<4>(sql_string, object.roleId, object.guildId, object.messageId, object.solution);
}


ChallengeRoleDTO ChallengeRoleRepository::get(size_t messageId) {
    static std::string sql_string{ "SELECT role_id, flag FROM challenge_roles WHERE message_id=$3" };

    ChallengeRoleDTO dto;
    dto.messageId = messageId;
    
    database::execQuery<4>(sql_string, dto.roleId, dto.guildId, dto.messageId, dto.solution);
    return dto;
}
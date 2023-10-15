#include "ChallengeRoleDTO.hpp"

#include "ChallengeRoleCommand.hpp"

ChallengeRoleDTO::ChallengeRoleDTO(size_t roleId, size_t guildId, size_t messageId, const std::string &solution)
	: roleId{roleId}, guildId{guildId}, messageId{messageId}, solution{solution} {
}

ChallengeRoleDTO::ChallengeRoleDTO(const RowDTOAdapter &row)
	: roleId{row.get<size_t>("role_Id")},
	  guildId{row.get<size_t>("guild_id")},
	  messageId{row.get<size_t>("message_id")},
	  solution{row.get<std::string>("flag")} {
}
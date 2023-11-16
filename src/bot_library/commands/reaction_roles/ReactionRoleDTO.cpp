#include "ReactionRoleDTO.hpp"

ReactionRoleDTO::ReactionRoleDTO(size_t role_id, size_t message_id, size_t guild_id, const std::string& emoji)
	: role_id{role_id}, message_id{message_id}, guild_id{guild_id}, emoji{emoji} {
}

ReactionRoleDTO::ReactionRoleDTO(const RowDTOAdapter& row)
	: role_id{row.get<size_t>("role_Id")},
	  message_id{row.get<size_t>("message_id")},
	  guild_id{row.get<size_t>("guild_id")},
	  emoji{row.get<std::string>("emoji")} {
}
#pragma once

#include <RowDTOAdapter.hpp>
#include <string>

class ReactionRoleDTO {
  public:
	size_t role_id;
	size_t message_id;
	size_t guild_id;
	std::string emoji;

	ReactionRoleDTO() = default;
	ReactionRoleDTO(size_t role_id, size_t message_id, size_t guild_id, const std::string& emoji);
	ReactionRoleDTO(const RowDTOAdapter& row);
};
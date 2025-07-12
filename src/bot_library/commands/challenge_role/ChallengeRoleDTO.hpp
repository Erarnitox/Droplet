/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description:
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/
 */

#pragma once

#include <RowDTOAdapter.hpp>
#include <string>

//-----------------------------------------------------
//
//-----------------------------------------------------
class ChallengeRoleDTO {
  public:
	size_t roleId;
	size_t guildId;
	size_t messageId;
	std::string solution;

	ChallengeRoleDTO() = default;
	ChallengeRoleDTO(size_t roleId, size_t guildId, size_t messageId, const std::string& solution);

	explicit ChallengeRoleDTO(const RowDTOAdapter& row);
};
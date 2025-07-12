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
class ChallengeBadgeDTO {
  public:
	std::string badge;
	size_t exp;
	size_t guildId;
	size_t messageId;
	std::string solution;
	std::string guild_name;

	ChallengeBadgeDTO() = default;
	ChallengeBadgeDTO(const std::string& badge,
					  size_t exp,
					  size_t guildId,
					  size_t messageId,
					  const std::string& solution,
					  const std::string& guild_name);

	explicit ChallengeBadgeDTO(const RowDTOAdapter& row);
};
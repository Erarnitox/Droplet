#pragma once

#include <RowDTOAdapter.hpp>
#include <string>

class ChallengeBadgeDTO {
  public:
	std::string badge;
	size_t exp;
	size_t guildId;
	size_t messageId;
	std::string solution;

	ChallengeBadgeDTO() = default;
	ChallengeBadgeDTO(const std::string& badge,
					  size_t exp,
					  size_t guildId,
					  size_t messageId,
					  const std::string& solution);
	ChallengeBadgeDTO(const RowDTOAdapter& row);
};
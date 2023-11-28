#include "ChallengeBadgeDTO.hpp"

#include "ChallengeBadgeCommand.hpp"

ChallengeBadgeDTO::ChallengeBadgeDTO(const std::string& badge, size_t exp, size_t guildId, size_t messageId, const std::string& solution)
	: badge{badge}, exp{exp}, guildId{guildId}, messageId{messageId}, solution{solution} {
}

ChallengeBadgeDTO::ChallengeBadgeDTO(const RowDTOAdapter& row)
	: badge{row.get<std::string>("badge")},
	  exp{row.get<size_t>("exp")},
	  guildId{row.get<size_t>("guild_id")},
	  messageId{row.get<size_t>("message_id")},
	  solution{row.get<std::string>("flag")} {
}
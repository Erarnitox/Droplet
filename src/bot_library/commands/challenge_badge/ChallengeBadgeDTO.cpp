#include "ChallengeBadgeDTO.hpp"

ChallengeBadgeDTO::ChallengeBadgeDTO(const std::string& badge,
									 size_t exp,
									 size_t guildId,
									 size_t messageId,
									 const std::string& solution,
									 const std::string& guild_name)
	: badge{badge}, exp{exp}, guildId{guildId}, messageId{messageId}, solution{solution}, guild_name{guild_name} {
}

ChallengeBadgeDTO::ChallengeBadgeDTO(const RowDTOAdapter& row)
	: badge{row.get<std::string>("badge")},
	  exp{row.get<size_t>("exp")},
	  guildId{row.get<size_t>("guild_id")},
	  messageId{row.get<size_t>("message_id")},
	  solution{row.get<std::string>("flag")},
	  guild_name{row.get<std::string>("guild_name")} {
}
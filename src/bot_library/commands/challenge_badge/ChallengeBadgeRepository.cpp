#include "ChallengeBadgeRepository.hpp"

#include <Database.hpp>
#include <cstddef>

#include "ChallengeBadgeDTO.hpp"

bool ChallengeBadgeRepository::create(const ChallengeBadgeDTO& object) {
	static std::string sql_string{
		"INSERT INTO challenge_badges(message_id, guild_id, badge_emoji, exp, flag) VALUES "
		"($1::int8, $2::int8, $3::varchar, $4::int8, $5::varchar)"};

	if (!Database::hasConnection()) {
		return false;
	}
	if (!object.messageId) {
		return false;
	}

	return database::execQuery(sql_string, object.messageId, object.guildId, object.badge, object.exp, object.solution);
}

bool ChallengeBadgeRepository::remove(size_t messageId) {
	static std::string sql_string{"DELETE FROM challenge_badges WHERE message_id = $1::int8"};

	if (!Database::hasConnection())
		return false;

	return database::execQuery(sql_string, messageId);
}

bool ChallengeBadgeRepository::update(const ChallengeBadgeDTO& object) {
	static std::string sql_string{
		"UPDATE challenge_badges(message_id, guild_id, badge_emoji, exp, flag) VALUES "
		"($1::int8, $2::int8, $3::varchar, $4::int8, $5::varchar) WHERE message_id = $1"};

	if (!Database::hasConnection()) {
		return false;
	}
	if (!object.messageId) {
		return false;
	}

	return database::execQuery(sql_string, object.messageId, object.guildId, object.badge, object.exp, object.solution);
}

ChallengeBadgeDTO ChallengeBadgeRepository::get(size_t messageId) {
	static std::string sql_string{"SELECT badge_emoji, exp, flag FROM challenge_roles WHERE message_id=$1::int8"};

	auto result{database::execSelect(sql_string, messageId)};

	ChallengeBadgeDTO dto;
	dto.messageId = messageId;
	dto.badge = result.get<decltype(dto.badge)>("badge_emoji");
	dto.exp = result.get<decltype(dto.exp)>("exp");
	dto.solution = result.get<decltype(dto.solution)>("flag");

	return dto;
}
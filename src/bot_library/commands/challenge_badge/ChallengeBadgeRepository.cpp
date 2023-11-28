#include "ChallengeBadgeRepository.hpp"

#include <Database.hpp>
#include <cstddef>
#include <vector>

#include "ChallengeBadgeDTO.hpp"

bool ChallengeBadgeRepository::create(const ChallengeBadgeDTO& object) {
	static std::string sql_string{
		"INSERT INTO challenge_roles(role_id, guild_id, message_id, flag) VALUES "
		"($1::int8, "
		"$2::int8, $3::int8, $4::varchar)"};

	if (!Database::hasConnection()) {
		return false;
	}
	if (!object.messageId) {
		return false;
	}

	return database::execQuery(sql_string, object.roleId, object.guildId, object.messageId, object.solution);
}

bool ChallengeBadgeRepository::remove(size_t messageId) {
	static std::string sql_string{"DELETE FROM challenge_roles WHERE message_id = $1::int8"};

	if (!Database::hasConnection())
		return false;

	return database::execQuery(sql_string, messageId);
}

bool ChallengeBadgeRepository::update(const ChallengeBadgeDTO& object) {
	static std::string sql_string{
		"UPDATE challenge_roles(role_id, guild_id, message_id, flag) VALUES "
		"($1::int8, "
		"$2::Int8, $3::int8, $4::varchar) WHERE message_id = $3"};

	if (!Database::hasConnection()) {
		return false;
	}
	if (!object.messageId) {
		return false;
	}

	return database::execQuery(sql_string, object.badge, object.exp, object.guildId, object.messageId, object.solution);
}

ChallengeBadgeDTO ChallengeBadgeRepository::get(size_t messageId) {
	static std::string sql_string{"SELECT role_id, flag FROM challenge_roles WHERE message_id=$1::int8"};

	auto result{database::execSelect(sql_string, messageId)};

	ChallengeBadgeDTO dto;
	dto.messageId = messageId;
	dto.badge = result.get<decltype(dto.badge)>("badge");
	dto.exp = result.get<decltype(dto.exp)>("exp");
	dto.solution = result.get<decltype(dto.solution)>("flag");

	return dto;
}
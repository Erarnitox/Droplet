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

#include "ChallengeBadgeRepository.hpp"

#include <Database.hpp>
#include <cstddef>

#include "ChallengeBadgeDTO.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
bool ChallengeBadgeRepository::create(const ChallengeBadgeDTO& object) noexcept {
	const static std::string sql_string{
		"INSERT INTO challenge_badges(message_id, guild_id, badge_emoji, exp, flag, guild_name) VALUES "
		"($1::int8, $2::int8, $3::varchar, $4::int8, $5::varchar, $6::varchar)"};

	if (not Database::hasConnection()) {
		return false;
	}
	if (not object.messageId) {
		return false;
	}

	return database::execQuery(
		sql_string, object.messageId, object.guildId, object.badge, object.exp, object.solution, object.guild_name);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool ChallengeBadgeRepository::remove(size_t messageId) noexcept {
	const static std::string sql_string{"DELETE FROM challenge_badges WHERE message_id = $1::int8"};

	if (not Database::hasConnection())
		return false;

	return database::execQuery(sql_string, messageId);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool ChallengeBadgeRepository::update(const ChallengeBadgeDTO& object) noexcept {
	const static std::string sql_string{
		"UPDATE challenge_badges(message_id, guild_id, badge_emoji, exp, flag, guild_name) VALUES "
		"($1::int8, $2::int8, $3::varchar, $4::int8, $5::varchar, $6::varchar) WHERE message_id = $1::int8"};

	if (not Database::hasConnection()) {
		return false;
	}
	if (not object.messageId) {
		return false;
	}

	return database::execQuery(
		sql_string, object.messageId, object.guildId, object.badge, object.exp, object.solution, object.guild_name);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
ChallengeBadgeDTO ChallengeBadgeRepository::get(size_t messageId) noexcept {
	const static std::string sql_string{
		"SELECT badge_emoji, exp, flag, guild_name FROM challenge_badges WHERE message_id=$1::int8"};

	const auto result{database::execSelect(sql_string, messageId)};

	ChallengeBadgeDTO dto;
	dto.messageId = messageId;
	dto.badge = result.get<decltype(dto.badge)>("badge_emoji");
	dto.exp = result.get<decltype(dto.exp)>("exp");
	dto.solution = result.get<decltype(dto.solution)>("flag");
	dto.guild_name = result.get<decltype(dto.guild_name)>("guild_name");

	return dto;
}
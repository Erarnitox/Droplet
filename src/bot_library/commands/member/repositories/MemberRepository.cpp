#include "MemberRepository.hpp"

#include <Database.hpp>
#include <cstddef>
#include <vector>

bool MemberRepository::create(const MemberDTO& object) {
	static std::string sql_string{
		"INSERT INTO members"
		"(discord_id, picture_url, discord) VALUES "
		"($1::int8, $2::varchar, $3::varchar)"};

	if (!Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string, object.discord_id, object.picture_url, object.username);
}

bool MemberRepository::remove(size_t id) {
	static std::string sql_string{"DELETE FROM members WHERE discord_id = $1::int8"};

	if (!Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string, id);
}

bool MemberRepository::update(const MemberDTO& object) {
	(void)object;
	/*
	static std::string sql_string{"UPDATE welcome_channels SET channel_id = $2::Int8 WHERE guild_id = $1"};

	if (!Database::hasConnection()) {
		return false;
	}

	if (!object.guild_id) {
		return false;
	}

	return database::execQuery(sql_string, object.guild_id, object.channel_id);
	*/
	return false;
}

MemberDTO MemberRepository::get(size_t id) {
	(void)id;
	/*
	static std::string sql_string{"SELECT channel_id FROM welcome_channels WHERE guild_id=$1::int8"};

	auto result{database::execSelect(sql_string, guild_id)};

	WelcomeChannelDTO dto;
	dto.guild_id = guild_id;
	dto.channel_id = result.get<decltype(dto.channel_id)>("channel_id");

	return dto;
	*/
	return {};
}
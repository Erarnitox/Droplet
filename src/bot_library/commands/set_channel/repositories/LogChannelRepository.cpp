#include "LogChannelRepository.hpp"

#include <Database.hpp>
#include <cstddef>
#include <vector>

#include "LogChannelDTO.hpp"

bool LogChannelRepository::create(const LogChannelDTO& object) {
	static std::string sql_string{"INSERT INTO log_channels(guild_id, channel_id) VALUES ($1::int8, $2::int8)"};

	if (!Database::hasConnection()) {
		return false;
	}

	if (!object.guild_id) {
		return false;
	}

	// log channel does already exist in the Database
	if (this->get(object.guild_id).channel_id != 0) {
		return false;
	}

	return database::execQuery(sql_string, object.guild_id, object.channel_id);
}

bool LogChannelRepository::remove(size_t guild_id) {
	static std::string sql_string{"DELETE FROM log_channels WHERE id = $1::int8"};

	if (!Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string, guild_id);
}

bool LogChannelRepository::update(const LogChannelDTO& object) {
	static std::string sql_string{"UPDATE log_channels SET channel_id = $2::Int8 WHERE guild_id = $1"};

	if (!Database::hasConnection()) {
		return false;
	}

	if (!object.guild_id) {
		return false;
	}

	return database::execQuery(sql_string, object.guild_id, object.channel_id);
}

LogChannelDTO LogChannelRepository::get(size_t guild_id) {
	static std::string sql_string{"SELECT guild_id FROM log_channels WHERE guild_id=$1::int8"};

	auto result{database::execSelect(sql_string, guild_id)};

	LogChannelDTO dto;
	dto.guild_id = guild_id;
	dto.channel_id = result.get<decltype(dto.channel_id)>("channel_id");

	return dto;
}
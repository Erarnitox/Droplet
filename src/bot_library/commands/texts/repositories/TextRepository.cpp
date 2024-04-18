#include "TextRepository.hpp"

#include <Database.hpp>
#include <cstddef>
#include <vector>

auto TextRepository::create(const TextDTO& object) -> bool {
	static std::string sql_string{
		"INSERT INTO papers"
		"(title, description, url, author) VALUES "
		"($1::varchar, $2::varchar, $3::varchar, $4::varchar)"};

	if (!Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string, object.title, object.description, object.url, object.creator);
}

auto TextRepository::remove(size_t id) -> bool {
	static std::string sql_string{"DELETE FROM papers WHERE id = $1::int8"};

	if (!Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string, id);
}

auto TextRepository::update(const TextDTO& object) -> bool {
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

auto TextRepository::get(size_t id) -> TextDTO {
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
#include "ResourceRepository.hpp"

#include <Database.hpp>
#include <cstddef>
#include <vector>

bool ResourceRepository::create(const ResourceDTO& object) {
	static std::string sql_string{
		"INSERT INTO resources"
		"(title, category, description, url, difficulty, guild_id, creator, creator_id) VALUES "
		"($1::varchar, $2::varchar, $3::varchar, $4::varchar, $5::int8, $6::int8, $7::varchar, $8::int8)"};

	if (!Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string,
							   object.title,
							   object.category,
							   object.description,
							   object.url,
							   object.difficulty,
							   object.guild_id,
							   object.creator,
							   object.creator_id);
}

bool ResourceRepository::remove(size_t id) {
	static std::string sql_string{"DELETE FROM resources WHERE id = $1::int8"};

	if (!Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string, id);
}

bool ResourceRepository::update(const ResourceDTO& object) {
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

ResourceDTO ResourceRepository::get(size_t id) {
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
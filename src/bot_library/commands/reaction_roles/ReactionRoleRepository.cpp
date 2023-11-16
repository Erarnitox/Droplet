#include "ReactionRoleRepository.hpp"

#include <Database.hpp>
#include <cstddef>
#include <vector>

#include "ReactionRoleDTO.hpp"

bool ReactionRoleRepository::create(const ReactionRoleDTO& object) {
	static std::string sql_string{
		"INSERT INTO reaction_roles(role_id, message_id, guild_id, emoji) VALUES "
		"($1::int8, "
		"$2::int8, $3::int8, $4::varchar)"};

	if (!Database::hasConnection()) {
		return false;
	}
	if (!object.message_id) {
		return false;
	}

	return database::execQuery(sql_string, object.role_id, object.message_id, object.guild_id, object.emoji);
}

bool ReactionRoleRepository::remove(size_t message_id) {
	static std::string sql_string{"DELETE FROM reaction_roles WHERE message_id = $1::int8"};

	if (!Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string, message_id);
}

bool ReactionRoleRepository::update(const ReactionRoleDTO& object) {
	static std::string sql_string{
		"UPDATE reaction_roles(role_id, message_id, guild_id, emoji) VALUES "
		"($1::int8, "
		"$2::Int8, $3::int8, $4::varchar) WHERE message_id = $3"};

	if (!Database::hasConnection()) {
		return false;
	}
	if (!object.message_id)
		return false;

	return database::execQuery(sql_string, object.role_id, object.message_id, object.guild_id, object.emoji);
}

ReactionRoleDTO ReactionRoleRepository::get(size_t message_id) {
	return {0, message_id, 0, ""};
}

ReactionRoleDTO ReactionRoleRepository::get(size_t message_id, const std::string& emoji) {
	static std::string sql_string{"SELECT role_id FROM reaction_roles WHERE message_id=$1::int8 AND emoji=$2::varchar"};

	auto result{database::execSelect(sql_string, message_id, emoji)};

	ReactionRoleDTO dto;
	dto.message_id = message_id;
	dto.role_id = result.get<decltype(dto.role_id)>("role_id");
	dto.emoji = emoji;

	return dto;
}
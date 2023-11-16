#include "ChallengeRoleRepository.hpp"

#include <Database.hpp>
#include <cstddef>
#include <vector>

#include "ChallengeRoleDTO.hpp"

bool ChallengeRoleRepository::create(const ChallengeRoleDTO& object) {
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

bool ChallengeRoleRepository::remove(size_t messageId) {
	static std::string sql_string{"DELETE FROM challenge_roles WHERE id = $1::int8"};

	if (!Database::hasConnection())
		return false;

	return database::execQuery(sql_string, messageId);
}

bool ChallengeRoleRepository::update(const ChallengeRoleDTO& object) {
	static std::string sql_string{
		"UPDATE challenge_roles(role_id, guild_id, message_id, flag) VALUES "
		"($1::int8, "
		"$2::Int8, $3::int8, $4::varchar) WHERE message_id = $3"};

	if (!Database::hasConnection())
		return false;
	if (!object.messageId)
		return false;
	if (this->get(object.messageId).guildId != 0)
		return false;

	return database::execQuery(sql_string, object.roleId, object.guildId, object.messageId, object.solution);
}

ChallengeRoleDTO ChallengeRoleRepository::get(size_t messageId) {
	static std::string sql_string{"SELECT role_id, flag FROM challenge_roles WHERE message_id=$1::int8"};

	auto result{database::execSelect(sql_string, messageId)};

	ChallengeRoleDTO dto;
	dto.messageId = messageId;
	dto.roleId = result.get<decltype(dto.roleId)>("role_id");
	dto.solution = result.get<decltype(dto.solution)>("flag");

	return dto;
}
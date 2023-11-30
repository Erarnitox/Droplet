#include "UserRepository.hpp"

#include <Database.hpp>
#include <cstddef>

#include "UserDTO.hpp"

bool UserRepository::create(const UserDTO& object) {
	static std::string sql_string{
		"INSERT INTO usr(user_id, color, exp, is_subscribed) VALUES "
		"($1::int8, $2::varchar, $3::int8, $4::int8)"};

	if (!Database::hasConnection()) {
		return false;
	}
	if (!object.user_id) {
		return false;
	}

	return database::execQuery(sql_string, object.user_id, object.color, object.exp, object.is_subscribed);
}

bool UserRepository::remove(size_t user_id) {
	static std::string sql_string{"DELETE FROM usr WHERE user_id = $1::int8"};

	if (!Database::hasConnection())
		return false;

	return database::execQuery(sql_string, user_id);
}

bool UserRepository::update(const UserDTO& object) {
	static std::string sql_string{
		"UPDATE usr(user_id, color, exp, is_subscribed) "
		"VALUES ($1::int8, $2::varchar, $3::int8, $4::int8) WHERE user_id = $1"};

	if (!Database::hasConnection()) {
		return false;
	}
	if (!object.user_id) {
		return false;
	}

	return database::execQuery(sql_string, object.user_id, object.color, object.exp, object.is_subscribed);
}

UserDTO UserRepository::get(size_t user_id) {
	static std::string sql_string{"SELECT color, exp, is_subscribed FROM usr WHERE user_id=$1::int8"};

	auto result{database::execSelect(sql_string, user_id)};

	UserDTO dto;
	dto.user_id = user_id;
	dto.color = result.get<decltype(dto.color)>("color");
	dto.exp = result.get<decltype(dto.exp)>("exp");
	dto.is_subscribed = result.get<decltype(dto.is_subscribed)>("is_subscribed");

	return dto;
}
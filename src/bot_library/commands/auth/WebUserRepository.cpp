#include "WebUserRepository.hpp"

#include <Database.hpp>
#include <cstddef>

#include "WebUserDTO.hpp"

bool WebUserRepository::create(const WebUserDTO& object) {
	static std::string sql_string{
		"INSERT INTO users(id, username, password, clearance, email, confirm_code, is_verified) VALUES "
		"($1::int8, $2::varchar, $3::varchar, $4::int8, $5::varchar, $6::bool)"};

	if (!Database::hasConnection()) {
		return false;
	}
	if (!object.id) {
		return false;
	}

	return database::execQuery(sql_string,
							   object.id,
							   object.username,
							   object.password,
							   object.clearance,
							   object.email,
							   object.confirm_code,
							   object.is_verified);
}

bool WebUserRepository::remove(size_t user_id) {
	static std::string sql_string{"DELETE FROM users WHERE id = $1::int8"};

	if (!Database::hasConnection())
		return false;

	return database::execQuery(sql_string, user_id);
}

bool WebUserRepository::update(const WebUserDTO& object) {
	static std::string sql_string{// TODO: make this make sense
								  "UPDATE users SET user_name = $2::varchar, color = $3::varchar, "
								  "exp = $4::int8, is_subscribed = $5::int8 "
								  "WHERE user_id = $1::int8"};

	if (!Database::hasConnection()) {
		return false;
	}
	if (!object.id) {
		return false;
	}

	return database::execQuery(sql_string,
							   object.username,
							   object.password,
							   object.clearance,
							   object.email,
							   object.confirm_code,
							   object.is_verified);
}

WebUserDTO WebUserRepository::get(size_t user_id) {
	static std::string sql_string{"SELECT * FROM users WHERE id=$1::int8"};

	auto result{database::execSelect(sql_string, user_id)};

	WebUserDTO dto;
	dto.id = user_id;
	dto.username = result.get<decltype(dto.username)>("username");
	dto.password = result.get<decltype(dto.password)>("password");
	dto.clearance = result.get<decltype(dto.clearance)>("clearance");
	dto.email = result.get<decltype(dto.email)>("email");
	dto.confirm_code = result.get<decltype(dto.confirm_code)>("confirm_code");
	dto.is_verified = result.get<decltype(dto.is_verified)>("is_verified");

	return dto;
}

WebUserDTO WebUserRepository::get(const std::string& username) {
	static std::string sql_string{"SELECT * FROM users WHERE username=$1::varchar"};

	auto result{database::execSelect(sql_string, username)};

	WebUserDTO dto;
	dto.id = result.get<decltype(dto.id)>("id");
	dto.username = result.get<decltype(dto.username)>("username");
	dto.password = result.get<decltype(dto.password)>("password");
	dto.clearance = result.get<decltype(dto.clearance)>("clearance");
	dto.email = result.get<decltype(dto.email)>("email");
	dto.confirm_code = result.get<decltype(dto.confirm_code)>("confirm_code");
	dto.is_verified = result.get<decltype(dto.is_verified)>("is_verified");

	return dto;
}
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

#include "UserRepository.hpp"

#include <DatabaseExecutor.hpp>
#include <cstddef>

#include "UserDTO.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
UserRepository::UserRepository() : executor_(DatabaseExecutor::application_instance()) {
}

UserRepository::UserRepository(DatabaseExecutor& executor) : executor_(executor) {
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool UserRepository::create(const UserDTO& object) noexcept {
	const static std::string sql_string{
		"INSERT INTO usr(user_id, user_name, color, exp, is_subscribed) VALUES "
		"($1::int8, $2::varchar, $3::varchar, $4::int8, $5::int8)"};

	if (not executor_.hasConnection()) {
		return false;
	}

	if (not object.user_id) {
		return false;
	}

	return executor_.execQuery(
		sql_string, object.user_id, object.user_name, object.color, object.exp, object.is_subscribed);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool UserRepository::remove(size_t user_id) noexcept {
	const static std::string sql_string{"DELETE FROM usr WHERE user_id = $1::int8"};

	if (not executor_.hasConnection())
		return false;

	return executor_.execQuery(sql_string, user_id);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool UserRepository::update(const UserDTO& object) noexcept {
	const static std::string sql_string{
		"UPDATE usr SET user_name = $2::varchar, color = $3::varchar, "
		"exp = $4::int8, is_subscribed = $5::int8 "
		"WHERE user_id = $1::int8"};

	if (not executor_.hasConnection()) {
		return false;
	}
	if (not object.user_id) {
		return false;
	}

	return executor_.execQuery(
		sql_string, object.user_id, object.user_name, object.color, object.exp, object.is_subscribed);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
UserDTO UserRepository::get(size_t user_id) const noexcept {
	const static std::string sql_string{"SELECT user_name, color, exp, is_subscribed FROM usr WHERE user_id=$1::int8"};

	const auto result{executor_.execSelect(sql_string, user_id)};

	UserDTO dto;
	dto.user_id = user_id;
	dto.user_name = result.get<decltype(dto.user_name)>("user_name");
	dto.color = result.get<decltype(dto.color)>("color");
	dto.exp = result.get<decltype(dto.exp)>("exp");
	dto.is_subscribed = result.get<decltype(dto.is_subscribed)>("is_subscribed");

	return dto;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
std::vector<UserDTO> UserRepository::getTopTen() const noexcept {
	const static std::string sql_string{
		std::string("SELECT user_id, user_name, color, exp "
					"    FROM usr "
					"ORDER BY exp DESC "
					"   LIMIT 10")};
	const auto result = executor_.execSelectAll(sql_string);

	std::vector<UserDTO> dtos;
	dtos.reserve(result.size());

	for (const auto& adapter : result) {
		UserDTO dto;

		dto.user_id = adapter.get<decltype(dto.user_id)>("user_id");
		dto.user_name = adapter.get<decltype(dto.user_name)>("user_name");
		dto.color = adapter.get<decltype(dto.color)>("color");
		dto.exp = adapter.get<decltype(dto.exp)>("exp");
		dto.is_subscribed = false;	// this probably should be confidential so...

		dtos.push_back(dto);
	}

	return dtos;
}
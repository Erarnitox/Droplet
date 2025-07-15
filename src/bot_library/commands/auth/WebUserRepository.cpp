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

#include "WebUserRepository.hpp"

#include <Database.hpp>
#include <cstddef>

#include "WebUserDTO.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
bool WebUserRepository::create(const WebUserDTO& object) noexcept {
	const static std::string sql_string{
		"INSERT INTO users(username, password, clearance, email, confirm_code, is_verified) VALUES "
		"($1::varchar, $2::varchar, $3::int4, $4::varchar, $5::varchar, $6::bool)"};

	if (not Database::hasConnection()) {
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

//-----------------------------------------------------
//
//-----------------------------------------------------
bool WebUserRepository::remove(size_t user_id) noexcept {
	const static std::string sql_string{"DELETE FROM users WHERE id = $1::int8"};

	if (not Database::hasConnection())
		return false;

	return database::execQuery(sql_string, user_id);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool WebUserRepository::update(const WebUserDTO& object) noexcept {
	const static std::string sql_string{
		"UPDATE users "
		"  SET username = $2::varchar, password = $3::varchar, "
		"      clearance = $4::int8, email = $5::varchar, "
		"      confirm_code = $6::varchar, is_verified = $7::bool "
		"WHERE id = $1::int8"};

	if (not Database::hasConnection()) {
		return false;
	}
	if (not object.id) {
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

//-----------------------------------------------------
//
//-----------------------------------------------------
WebUserDTO WebUserRepository::get(size_t user_id) const noexcept {
	const static std::string sql_string{
		"SELECT username, password, clearance, email, confirm_code, is_verified FROM users WHERE id=$1::int8"};

	const auto result{database::execSelect(sql_string, user_id)};

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

//-----------------------------------------------------
//
//-----------------------------------------------------
WebUserDTO WebUserRepository::get(const std::string& username) const noexcept {
	const static std::string sql_string{
		"SELECT id, password, clearance, email, confirm_code, is_verified FROM users WHERE username=$1::varchar"};

	const auto result{database::execSelect(sql_string, username)};

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

//-----------------------------------------------------
//
//-----------------------------------------------------
std::vector<WebUserDTO> WebUserRepository::getAll() const noexcept {
	const static std::string sql_string{
		"SELECT id, username, password, clearance, email, confirm_code, is_verified FROM users"};

	const auto result{database::execSelectAll(sql_string)};

	std::vector<WebUserDTO> dtos;
	dtos.reserve(result.size());

	for (const auto& adapter : result) {
		WebUserDTO dto;
		dto.id = adapter.get<decltype(dto.id)>("id");
		dto.username = adapter.get<decltype(dto.username)>("username");
		dto.password = adapter.get<decltype(dto.password)>("password");
		dto.clearance = adapter.get<decltype(dto.clearance)>("clearance");
		dto.email = adapter.get<decltype(dto.email)>("email");
		dto.confirm_code = adapter.get<decltype(dto.confirm_code)>("confirm_code");
		dto.is_verified = adapter.get<decltype(dto.is_verified)>("is_verified");

		dtos.push_back(dto);
	}

	return dtos;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool WebUserRepository::exists(const std::string& username) const noexcept {
	const auto& dto{get(username)};
	return dto.id != 0;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool WebUserRepository::verify(const std::string& token, size_t clearance) noexcept {
	const static std::string sql_string{
		"SELECT id, username, password, clearance, email, is_verified FROM users WHERE confirm_code=$1::varchar"};

	const auto result{database::execSelect(sql_string, token)};

	WebUserDTO dto;
	dto.id = result.get<decltype(dto.id)>("id");
	dto.username = result.get<decltype(dto.username)>("username");
	dto.password = result.get<decltype(dto.password)>("password");
	dto.clearance = clearance;
	dto.email = result.get<decltype(dto.email)>("email");
	dto.confirm_code = "";
	dto.is_verified = true;

	return update(dto);
}
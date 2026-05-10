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

#include "HasBadgeRepository.hpp"

#include <DatabaseExecutor.hpp>

#include "RowDTOAdapter.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
HasBadgeRepository::HasBadgeRepository() : executor_(DatabaseExecutor::application_instance()) {
}

HasBadgeRepository::HasBadgeRepository(DatabaseExecutor& executor) : executor_(executor) {
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool HasBadgeRepository::create(size_t user_id, size_t message_id) noexcept {
	const static std::string sql_string{
		"INSERT INTO has_badge(user_id, message_id) VALUES "
		"($1::int8, $2::int8)"};

	if (not executor_.hasConnection()) {
		return false;
	}

	if (not(user_id && message_id)) {
		return false;
	}

	return executor_.execQuery(sql_string, user_id, message_id);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool HasBadgeRepository::remove(size_t user_id, size_t message_id) noexcept {
	std::string sql_string{"DELETE FROM has_badge WHERE user_id = $1::int8"};

	if (message_id) {
		sql_string = "DELETE FROM has_badge WHERE user_id = $1::int8 AND message_id = $2::int8";
	}

	if (not executor_.hasConnection()) {
		return false;
	}

	return executor_.execQuery(sql_string, user_id, message_id);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
std::vector<size_t> HasBadgeRepository::get(size_t user_id) const noexcept {
	const static std::string sql_string{"SELECT message_id FROM has_badge WHERE user_id=$1::int8"};

	const auto role_dtos{executor_.execSelectAll(sql_string, user_id)};
	std::vector<size_t> roles;

	for (const auto& dto : role_dtos) {
		roles.push_back(dto.get<size_t>("message_id"));
	}

	return roles;
}
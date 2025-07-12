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

#include "NotificationRepository.hpp"

#include <Database.hpp>
#include <cstddef>
#include <vector>

#include "NotificationDTO.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
bool NotificationRepository::create(const NotificationDTO& object) noexcept {
	const static std::string sql_string{
		"INSERT INTO notifications"
		"(guild_id, channel_id, type, data, message, timestep) VALUES "
		"($1::int8, $2::int8, $3::varchar, $4::varchar, $5::varchar, $6::int8)"};

	if (not Database::hasConnection()) {
		return false;
	}

	return database::execQuery(
		sql_string, object.guild_id, object.channel_id, object.type, object.data, object.message, object.timestep);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool NotificationRepository::remove(size_t id) noexcept {
	const static std::string sql_string{"DELETE FROM notifications WHERE guild_id = $1::int8"};

	if (not Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string, id);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool NotificationRepository::update(const NotificationDTO& object) noexcept {
	const static std::string sql_string{
		"UPDATE notifications SET channel_id = $2::int8, type = $3::varchar, data = $4::varchar, message = "
		"$5::varchar, "
		"timestep = $6::int8 WHERE guild_id = $1::int8"};

	if (not Database::hasConnection()) {
		return false;
	}

	if (not object.guild_id) {
		return false;
	}

	return database::execQuery(
		sql_string, object.guild_id, object.channel_id, object.type, object.data, object.message, object.timestep);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
NotificationDTO NotificationRepository::get(size_t id) noexcept {
	const static std::string sql_string{"SELECT channel_id FROM notifications WHERE guild_id=$1::int8"};

	const auto result{database::execSelect(sql_string, id)};

	NotificationDTO dto;
	dto.guild_id = id;
	dto.channel_id = result.get<decltype(dto.channel_id)>("channel_id");

	return dto;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
std::vector<NotificationDTO> NotificationRepository::getAll() noexcept {
	const static std::string sql_string{
		"SELECT guild_id, channel_id, type, data, message, timestep FROM notifications"};

	const auto result{database::execSelectAll(sql_string)};

	std::vector<NotificationDTO> dtos;
	dtos.reserve(result.size());

	for (const auto& adapter : result) {
		NotificationDTO dto;
		dto.guild_id = adapter.get<decltype(dto.guild_id)>("guild_id");
		dto.channel_id = adapter.get<decltype(dto.channel_id)>("channel_id");
		dto.timestep = adapter.get<decltype(dto.timestep)>("timestep");
		dto.type = adapter.get<decltype(dto.type)>("type");
		dto.data = adapter.get<decltype(dto.data)>("data");
		dto.message = adapter.get<decltype(dto.message)>("message");

		dtos.push_back(dto);
	}

	return dtos;
}
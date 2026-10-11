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

#include <DatabaseExecutor.hpp>
#include <cstddef>
#include <vector>

#include "NotificationDTO.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
NotificationRepository::NotificationRepository(DatabaseExecutor& executor) : executor_(executor) {
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool NotificationRepository::ensure_broadcast_column() const noexcept {
	const static std::string sql_string{
		"ALTER TABLE public.notifications ADD COLUMN IF NOT EXISTS allow_broadcast boolean NOT NULL DEFAULT false"};

	if (not executor_.hasConnection()) {
		return false;
	}

	return executor_.execQuery(sql_string);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool NotificationRepository::create(const NotificationDTO& object) noexcept {
	const static std::string sql_string{
		"INSERT INTO notifications"
		"(guild_id, channel_id, type, data, message, timestep, allow_broadcast) VALUES "
		"($1::int8, $2::int8, $3::varchar, $4::varchar, $5::varchar, $6::int8, $7::boolean)"};

	if (not executor_.hasConnection() || not ensure_broadcast_column()) {
		return false;
	}

	return executor_.execQuery(sql_string,
							   object.guild_id,
							   object.channel_id,
							   object.type,
							   object.data,
							   object.message,
							   object.timestep,
							   object.allow_broadcast);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool NotificationRepository::remove(size_t id) noexcept {
	const static std::string sql_string{"DELETE FROM notifications WHERE guild_id = $1::int8"};

	if (not executor_.hasConnection()) {
		return false;
	}

	return executor_.execQuery(sql_string, id);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool NotificationRepository::update(const NotificationDTO& object) noexcept {
	const static std::string sql_string{
		"UPDATE notifications SET channel_id = $2::int8, type = $3::varchar, data = $4::varchar, message = "
		"$5::varchar, "
		"timestep = $6::int8, allow_broadcast = $7::boolean WHERE guild_id = $1::int8"};

	if (not executor_.hasConnection() || not ensure_broadcast_column()) {
		return false;
	}

	if (not object.guild_id) {
		return false;
	}

	return executor_.execQuery(sql_string,
							   object.guild_id,
							   object.channel_id,
							   object.type,
							   object.data,
							   object.message,
							   object.timestep,
							   object.allow_broadcast);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
NotificationDTO NotificationRepository::get(size_t id) const noexcept {
	const static std::string sql_string{
		"SELECT channel_id, type, data, message, timestep, allow_broadcast FROM notifications WHERE guild_id=$1::int8"};

	if (not ensure_broadcast_column()) {
		return {};
	}

	const auto result{executor_.execSelect(sql_string, id)};

	NotificationDTO dto;
	dto.guild_id = id;
	dto.channel_id = result.get<decltype(dto.channel_id)>("channel_id");
	dto.timestep = result.get<decltype(dto.timestep)>("timestep");
	dto.type = result.get<decltype(dto.type)>("type");
	dto.data = result.get<decltype(dto.data)>("data");
	dto.message = result.get<decltype(dto.message)>("message");
	dto.allow_broadcast = result.get<bool>("allow_broadcast");

	return dto;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
std::vector<NotificationDTO> NotificationRepository::getAll() const noexcept {
	const static std::string sql_string{
		"SELECT guild_id, channel_id, type, data, message, timestep, allow_broadcast FROM notifications"};

	if (not ensure_broadcast_column()) {
		return {};
	}

	const auto result{executor_.execSelectAll(sql_string)};

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
		dto.allow_broadcast = adapter.get<bool>("allow_broadcast");

		dtos.push_back(dto);
	}

	return dtos;
}
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

#include "PortalRepository.hpp"

#include <Database.hpp>
#include <cstddef>
#include <string>
#include <vector>

#include "PortalDTO.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
bool PortalRepository::create(const PortalDTO& object) noexcept {
	const static std::string sql_string{
		"INSERT INTO portals"
		"(guild_id, channel_id) VALUES "
		"($1::int8, $2::int8)"};

	if (not Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string, object.guild_id, object.channel_id);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool PortalRepository::remove(size_t id) noexcept {
	const static std::string sql_string{"DELETE FROM portals WHERE guild_id = $1::int8"};

	if (not Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string, id);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool PortalRepository::update(const PortalDTO& object) noexcept {
	const std::string sql_string{"UPDATE portals SET channel_id = " + std::to_string(object.channel_id) +
								 " WHERE guild_id = " + std::to_string(object.guild_id)};

	if (not Database::hasConnection()) {
		return false;
	}

	if (not object.guild_id) {
		return false;
	}

	return database::execQuery(sql_string);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
PortalDTO PortalRepository::get(size_t id) const noexcept {
	const std::string sql_string{"SELECT channel_id FROM portals WHERE guild_id=" + std::to_string(id)};

	const auto result{database::execSelect(sql_string)};

	PortalDTO dto;
	dto.guild_id = id;
	dto.channel_id = result.get<decltype(dto.channel_id)>("channel_id");

	return dto;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
std::vector<PortalDTO> PortalRepository::getAll() const noexcept {
	const static std::string sql_string{"SELECT guild_id, channel_id FROM portals"};
	const auto result{database::execSelectAll(sql_string)};

	std::vector<PortalDTO> dtos;
	dtos.reserve(result.size());

	for (const auto& adapter : result) {
		PortalDTO dto;
		dto.guild_id = adapter.get<decltype(dto.guild_id)>("guild_id");
		dto.channel_id = adapter.get<decltype(dto.channel_id)>("channel_id");
		dtos.push_back(dto);
	}

	return dtos;
}
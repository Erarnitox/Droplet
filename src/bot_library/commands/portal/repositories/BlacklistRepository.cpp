#include "BlacklistRepository.hpp"

#include <Database.hpp>
#include <cstddef>
#include <vector>

#include "BlacklistDTO.hpp"

bool BlacklistRepository::create(const BlacklistDTO& object) noexcept {
	const static std::string sql_string{
		"INSERT INTO blacklist"
		"(username) VALUES "
		"($1::varchar)"};

	if (not Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string, object.username);
}

bool BlacklistRepository::remove(size_t id) noexcept {
	const static std::string sql_string{"DELETE FROM blacklist WHERE id = $1::int8"};

	if (not Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string, id);
}

bool BlacklistRepository::update(const BlacklistDTO& object) noexcept {
	(void)object;
	return false;
}

BlacklistDTO BlacklistRepository::get(size_t id) noexcept {
	(void)id;
	return {};
}

std::vector<BlacklistDTO> BlacklistRepository::getAll() noexcept {
	const static std::string sql_string{"SELECT username FROM blacklist"};
	const auto result{database::execSelectAll(sql_string)};

	std::vector<BlacklistDTO> dtos;
	dtos.reserve(result.size());

	for (const auto& adapter : result) {
		BlacklistDTO dto;
		dto.username = adapter.get<decltype(dto.username)>("username");
		dtos.push_back(dto);
	}

	return dtos;
}
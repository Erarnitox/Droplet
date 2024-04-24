#include "BlacklistRepository.hpp"

#include <Database.hpp>
#include <cstddef>
#include <vector>

#include "BlacklistDTO.hpp"

auto BlacklistRepository::create(const BlacklistDTO& object) -> bool {
	static std::string sql_string{
		"INSERT INTO blacklist"
		"(username) VALUES "
		"($1::varchar)"};

	if (!Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string, object.username);
}

auto BlacklistRepository::remove(size_t id) -> bool {
	static std::string sql_string{"DELETE FROM blacklist WHERE id = $1::int8"};

	if (!Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string, id);
}

auto BlacklistRepository::update(const BlacklistDTO& object) -> bool {
	(void)object;
	return false;
}

auto BlacklistRepository::get(size_t id) -> BlacklistDTO {
	(void)id;
	return {};
}

auto BlacklistRepository::getAll() -> std::vector<BlacklistDTO> {
	static std::string sql_string{"SELECT username FROM blacklist"};
	auto result{database::execSelectAll(sql_string)};

	std::vector<BlacklistDTO> dtos;
	dtos.reserve(result.size());

	for (const auto& adapter : result) {
		BlacklistDTO dto;
		dto.username = adapter.get<decltype(dto.username)>("username");
		dtos.push_back(dto);
	}

	return dtos;
}
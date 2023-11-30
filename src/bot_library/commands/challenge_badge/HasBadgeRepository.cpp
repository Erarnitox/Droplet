#include "HasBadgeRepository.hpp"

#include <Database.hpp>

#include "RowDTOAdapter.hpp"

bool HasBadgeRepository::create(size_t user_id, size_t message_id) {
	static std::string sql_string{
		"INSERT INTO has_badge(user_id, message_id) VALUES "
		"($1::int8, $2::int8)"};

	if (!Database::hasConnection()) {
		return false;
	}
	if (!(user_id && message_id)) {
		return false;
	}

	return database::execQuery(sql_string, user_id, message_id);
}

bool HasBadgeRepository::remove(size_t user_id, size_t message_id) {
	static std::string sql_string{"DELETE FROM has_badge WHERE user_id = $1::int8"};

	if (message_id) {
		sql_string = "DELETE FROM has_badge WHERE user_id = $1::int8 AND message_id = $2::int8";
	}

	if (!Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string, user_id, message_id);
}

std::vector<size_t> HasBadgeRepository::get(size_t user_id) {
	static std::string sql_string{"SELECT message_id FROM has_badge WHERE user_id=$1::int8"};

	// auto result{database::execSelect(sql_string, user_id)};
	std::vector<size_t> roles;

	{  // Database Transaction Block
		pqxx::work txn(*Database::getConnection());
		pqxx::result result = txn.exec_params(sql_string, user_id);
		txn.commit();

		for (const auto& row : result) {
			const auto dto{RowDTOAdapter(row)};
			roles.push_back(dto.get<size_t>("message_id"));
		}
	}

	return roles;
}
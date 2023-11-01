#pragma once

#include <cstddef>
#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <string>
#include <typeindex>
#include <vector>

#include "RowDTOAdapter.hpp"

class Database {
  public:
	Database() = delete;

	[[nodiscard]] static auto connect(const std::string& db_name,
									  const std::string& user,
									  const std::string& password,
									  const std::string& host,
									  const std::string& port) -> bool;

	[[nodiscard]] static auto connect(const std::string& connection_string) -> bool;

	static auto hasConnection() noexcept -> bool;
	static auto reconnect() noexcept -> void;
	static auto disconnect() noexcept -> void;
	static auto getConnection() noexcept -> pqxx::connection*;

	// TODO: to be removed
	[[nodiscard]] static auto get_reaction_role_data(size_t message_id, const std::string& reaction_emoji) noexcept
		-> size_t;

	static auto insert_reaction_role_data(const std::string& role_id,
										  size_t guild_id,
										  const std::string& message_id,
										  const std::string& emoji) noexcept -> void;

	// TODO: to be removed
	[[nodiscard]] static auto get_welcome_channel_id(size_t guild_id) noexcept -> size_t;
	static auto insert_welcome_channel_id(size_t guild_id, size_t channel_id) noexcept -> void;

	// TODO: to be removed
	[[nodiscard]] static auto get_goodbye_channel_id(size_t guild_id) noexcept -> size_t;
	static auto insert_goodbye_channel_id(size_t guild_id, size_t channel_id) noexcept -> void;

	// TODO: to be removed
	[[nodiscard]] static auto get_log_channel_id(size_t guild_id) noexcept -> size_t;
	static auto insert_log_channel_id(size_t guild_id, size_t channel_id) noexcept -> void;
};

namespace database {

// Compile time "for" loop
template <size_t I = 0, typename... Types>
constexpr void assignResults(const pqxx::result& result, std::vector<std::variant<Types...>>& args) {
	if constexpr (I < sizeof...(Types)) {
		args[I] = result.at(0, I).template get<std::variant_alternative_t<I, std::variant<Types...>>>().value();
		assignResults<I + 1>(result, args);
	}
}

template <typename... Types>
[[nodiscard("You need to check if the Query was executed on the Database!")]] auto execQuery(const std::string& query,
																							 Types&&... args) noexcept
	-> bool {
	static int times = 0;
	try {
		pqxx::work txn(*Database::getConnection());

		// perform the database transaction
		pqxx::result result = txn.exec_params(query, std::forward<Types>(args)...);
		txn.commit();

		times = 0;
		return true;
	} catch (const pqxx::broken_connection& e) {
		++times;
		if (times > 10) {
			times = 0;
			return false;
		}
		Database::reconnect();
		return execQuery(query, args...);
	} catch (...) {
		return false;
	}
}

template <typename... Types>
[[nodiscard]] auto execSelect(const std::string& query, Types&&... args) noexcept -> RowDTOAdapter {
	static int times = 0;
	try {
		pqxx::work txn(*Database::getConnection());

		// perform the database transaction
		pqxx::result result = txn.exec_params(query, std::forward<Types>(args)...);
		txn.commit();

		times = 0;

		return {result[0]};
	} catch (const pqxx::broken_connection& e) {
		++times;
		if (times > 10) {
			times = 0;
			return {pqxx::row()};
		}
		Database::reconnect();
		return execSelect(query, args...);
	}
	/*
	catch (...) {
									   return false;
	}*/
}
}  // namespace database
#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "ConnectionPool.hpp"
#include "RowDTOAdapter.hpp"

/**
 * Executes queries/transactions against a leased connection from ConnectionPool.
 */
class DatabaseExecutor {
  public:
	explicit DatabaseExecutor(ConnectionPool& pool) noexcept : pool_(pool) {
	}

	DatabaseExecutor(const DatabaseExecutor&) = delete;
	DatabaseExecutor& operator=(const DatabaseExecutor&) = delete;

	[[nodiscard]] bool hasConnection() const noexcept {
		return pool_.has_connection();
	}

	template <typename... Types>
	[[nodiscard]] bool execQuery(const std::string& query, Types&&... args);

	template <typename... Types>
	[[nodiscard]] RowDTOAdapter execSelect(const std::string& query, Types&&... args);

	template <typename... Types>
	[[nodiscard]] std::vector<RowDTOAdapter> execSelectAll(const std::string& query, Types&&... args);

  private:
	ConnectionPool& pool_;
};

template <typename... Types>
[[nodiscard]] bool DatabaseExecutor::execQuery(const std::string& query, Types&&... args) {
	int times = 0;
	while (times <= 10) {
		auto lease = pool_.acquire();
		if (not lease) {
			return false;
		}
		try {
			NativeDatabase::Transaction txn(lease.get());
			const NativeDatabase::ParameterList params{std::forward<Types>(args)...};
			(void)txn.exec(query, params);
			txn.commit();
			return true;
		} catch (const NativeDatabase::BrokenConnectionException&) {
			lease.discard();
			++times;
		} catch (const std::exception& e) {
			std::cerr << "Error: " << e.what() << "\nInvalid exec for query:\n" << query << '\n';
			return false;
		}
	}
	return false;
}

template <typename... Types>
[[nodiscard]] RowDTOAdapter DatabaseExecutor::execSelect(const std::string& query, Types&&... args) {
	int times = 0;
	while (times <= 10) {
		auto lease = pool_.acquire();
		if (not lease) {
			return {NativeDatabase::Row{}};
		}
		try {
			NativeDatabase::Transaction txn(lease.get());
			const NativeDatabase::ParameterList params{std::forward<Types>(args)...};
			const NativeDatabase::Result result = txn.exec(query, params);
			txn.commit();
			if (result.empty()) {
				return {NativeDatabase::Row{}};
			}
			return {result[0]};
		} catch (const NativeDatabase::BrokenConnectionException&) {
			lease.discard();
			++times;
		} catch (const std::exception& e) {
			std::cerr << "Error: " << e.what() << "\nInvalid selection for query:\n" << query << '\n';
			return {NativeDatabase::Row{}};
		}
	}
	return {NativeDatabase::Row{}};
}

template <typename... Types>
[[nodiscard]] std::vector<RowDTOAdapter> DatabaseExecutor::execSelectAll(const std::string& query, Types&&... args) {
	int times = 0;
	while (times <= 10) {
		auto lease = pool_.acquire();
		if (not lease) {
			return {};
		}
		try {
			NativeDatabase::Transaction txn(lease.get());
			const NativeDatabase::ParameterList params{std::forward<Types>(args)...};
			const NativeDatabase::Result result = txn.exec(query, params);
			txn.commit();
			std::vector<RowDTOAdapter> selection;
			selection.reserve(static_cast<std::size_t>(result.size()));
			for (const auto& row : result) {
				selection.emplace_back(row);
			}
			return selection;
		} catch (const NativeDatabase::BrokenConnectionException&) {
			lease.discard();
			++times;
		} catch (const std::exception& e) {
			std::cerr << "Error: " << e.what() << "\nInvalid selection for query:\n" << query << '\n';
			return {};
		}
	}
	return {};
}

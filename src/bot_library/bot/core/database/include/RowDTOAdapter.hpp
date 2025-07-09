/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: This class represents a generic
 * 	DTO Interface. That can be used by other DTOs.
 * 	It acts as an adapter to the pqxx rows and isolated the
 * 	dependency on the pqxx library away from the DTOs
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/RowDTOAdapter
 */

#pragma once

#include "DatabaseBackend.hpp"

// Adapter class to convert DatabaseRow to GenericDTO
class RowDTOAdapter {
  private:
	const NativeDatabase::Row row_;

  public:
	RowDTOAdapter(const NativeDatabase::Row& row) : row_(row) {
	}

	/**
	 * @brief get the value of a field with the specified name
	 *
	 * @param field_name the name of the field in the database row
	 * @return returns the value in the database field.
	 * If the name is not found it returns an empty object of
	 * the expected type
	 */
	template <typename T>
	T get(const std::string& field_name) const {
		try {
			return this->row_[field_name].as<T>();
		} catch (...) {
			return T{};
		}
	}
};
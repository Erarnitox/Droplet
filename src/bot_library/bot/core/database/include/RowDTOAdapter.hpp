/**
 *  (c) Copyright dropsoft.org - All rights reserved
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
 **/

#pragma once

#include <pqxx/pqxx>

// Adapter class to convert pqxx::row to GenericDTO
class RowDTOAdapter {
  private:
	const pqxx::row row_;

  public:
	RowDTOAdapter(const pqxx::row& row) : row_(row) {
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
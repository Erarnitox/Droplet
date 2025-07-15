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

#pragma once

#include <IRepository.hpp>

#include "UserDTO.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
class UserRepository : public IRepository<UserDTO> {
  public:
	[[nodiscard]] bool create(const UserDTO& object) noexcept override;

	[[nodiscard]] bool remove(size_t user_id) noexcept override;

	[[nodiscard]] UserDTO get(size_t user_id) const noexcept override;

	[[nodiscard]] bool update(const UserDTO& object) noexcept override;

	[[nodiscard]] std::vector<UserDTO> getTopTen() const noexcept;
};
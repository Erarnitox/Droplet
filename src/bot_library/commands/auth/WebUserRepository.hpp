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
#include <cstddef>

#include "WebUserDTO.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
class WebUserRepository : public IRepository<WebUserDTO> {
  public:
	[[nodiscard]] bool create(const WebUserDTO& object) noexcept override;

	[[nodiscard]] bool remove(size_t user_id) noexcept override;

	[[nodiscard]] WebUserDTO get(size_t user_id) const noexcept override;

	[[nodiscard]] WebUserDTO get(const std::string& username) const noexcept;

	[[nodiscard]] std::vector<WebUserDTO> getAll() const noexcept;

	[[nodiscard]] bool verify(const std::string& token, size_t clearance) noexcept;

	[[nodiscard]] bool exists(const std::string& username) const noexcept;

	[[nodiscard]] bool update(const WebUserDTO& object) noexcept override;
};
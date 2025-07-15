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

#include <vector>

#include "BlacklistDTO.hpp"
#include "IRepository.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
class BlacklistRepository : public IRepository<BlacklistDTO> {
  public:
	[[nodiscard]] bool create(const BlacklistDTO& object) noexcept override;

	[[nodiscard]] bool remove(size_t id) noexcept override;

	[[nodiscard]] BlacklistDTO get(size_t id) const noexcept override;

	[[nodiscard]] bool update(const BlacklistDTO& object) noexcept override;

	[[nodiscard]] std::vector<BlacklistDTO> getAll() const noexcept;
};
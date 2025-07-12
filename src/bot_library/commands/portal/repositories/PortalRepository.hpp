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

#include "IRepository.hpp"
#include "PortalDTO.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
class PortalRepository : public IRepository<PortalDTO> {
  public:
	[[nodiscard]] bool create(const PortalDTO& object) noexcept override;

	[[nodiscard]] bool remove(size_t id) noexcept override;

	[[nodiscard]] PortalDTO get(size_t id) noexcept override;

	[[nodiscard]] bool update(const PortalDTO& object) noexcept override;

	[[nodiscard]] std::vector<PortalDTO> getAll() noexcept;
};
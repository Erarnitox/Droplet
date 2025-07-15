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

#include "ResourceDTO.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
class ResourceRepository : public IRepository<ResourceDTO> {
  public:
	[[nodiscard]] bool create(const ResourceDTO& object) noexcept override;

	[[nodiscard]] bool remove(size_t id) noexcept override;

	[[nodiscard]] ResourceDTO get(size_t id) const noexcept override;

	[[nodiscard]] std::vector<ResourceDTO> get(const std::string& category) const noexcept;

	[[nodiscard]] bool update(const ResourceDTO& object) noexcept override;
};
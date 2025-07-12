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

#include "ChallengeBadgeDTO.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
class ChallengeBadgeRepository : public IRepository<ChallengeBadgeDTO> {
  public:
	[[nodiscard]] bool create(const ChallengeBadgeDTO& object) noexcept override;

	[[nodiscard]] bool remove(size_t messageId) noexcept override;

	[[nodiscard]] ChallengeBadgeDTO get(size_t messageId) noexcept override;

	[[nodiscard]] bool update(const ChallengeBadgeDTO& object) noexcept override;
};
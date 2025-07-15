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

//-----------------------------------------------------
//
//-----------------------------------------------------
class HasBadgeRepository {
  public:
	[[nodiscard]] bool create(size_t user_id, size_t message_id) noexcept;

	[[nodiscard]] bool remove(size_t user_id, size_t message_id = 0) noexcept;
	[[nodiscard]] bool removeBadge(size_t message_id) noexcept;

	[[nodiscard]] std::vector<size_t> get(size_t user_id) const noexcept;
};
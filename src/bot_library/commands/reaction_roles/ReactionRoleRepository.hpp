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

#include "ReactionRoleDTO.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
class ReactionRoleRepository : public IRepository<ReactionRoleDTO> {
  public:
	[[nodiscard]] bool create(const ReactionRoleDTO& object) noexcept override;

	[[nodiscard]] bool remove(size_t message_id) noexcept override;

	[[nodiscard]] ReactionRoleDTO get(size_t message_id) const noexcept override;

	[[nodiscard]] ReactionRoleDTO get(size_t message_id, const std::string& emoji) const noexcept;

	[[nodiscard]] bool update(const ReactionRoleDTO& object) noexcept override;
};
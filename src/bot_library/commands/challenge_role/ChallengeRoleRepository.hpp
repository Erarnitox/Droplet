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

#include "ChallengeRoleDTO.hpp"

class DatabaseExecutor;

//-----------------------------------------------------
//
//-----------------------------------------------------
class ChallengeRoleRepository : public IRepository<ChallengeRoleDTO> {
  public:
	ChallengeRoleRepository();
	explicit ChallengeRoleRepository(DatabaseExecutor& executor);

	[[nodiscard]] bool create(const ChallengeRoleDTO& object) noexcept override;

	[[nodiscard]] bool remove(size_t messageId) noexcept override;

	[[nodiscard]] ChallengeRoleDTO get(size_t messageId) const noexcept override;

	[[nodiscard]] bool update(const ChallengeRoleDTO& object) noexcept override;

  private:
	DatabaseExecutor& executor_;
};
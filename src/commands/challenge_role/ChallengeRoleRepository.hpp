#pragma once

#include <IRepository.hpp>

#include "ChallengeRoleDTO.hpp"

class ChallengeRoleRepository : public IRepository<ChallengeRoleDTO> {
  public:
	[[nodiscard]] bool create(const ChallengeRoleDTO& object) override;

	[[nodiscard]] bool remove(size_t messageId) override;

	[[nodiscard]] ChallengeRoleDTO get(size_t messageId) override;

	[[nodiscard]] bool update(const ChallengeRoleDTO& object) override;
};
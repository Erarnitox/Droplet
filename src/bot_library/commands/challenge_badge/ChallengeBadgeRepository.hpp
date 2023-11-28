#pragma once

#include <IRepository.hpp>

#include "ChallengeBadgeDTO.hpp"

class ChallengeBadgeRepository : public IRepository<ChallengeBadgeDTO> {
  public:
	[[nodiscard]] bool create(const ChallengeBadgeDTO& object) override;

	[[nodiscard]] bool remove(size_t messageId) override;

	[[nodiscard]] ChallengeBadgeDTO get(size_t messageId) override;

	[[nodiscard]] bool update(const ChallengeBadgeDTO& object) override;
};
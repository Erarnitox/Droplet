#pragma once

#include <IRepository.hpp>

#include "WelcomeChannelDTO.hpp"

class WelcomeChannelRepository : public IRepository<WelcomeChannelDTO> {
  public:
	[[nodiscard]] bool create(const WelcomeChannelDTO& object) override;

	[[nodiscard]] bool remove(size_t guild_id) override;

	[[nodiscard]] WelcomeChannelDTO get(size_t guild_id) override;

	[[nodiscard]] bool update(const WelcomeChannelDTO& object) override;
};
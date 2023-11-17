#pragma once

#include <IRepository.hpp>

#include "GoodbyeChannelDTO.hpp"

class GoodbyeChannelRepository : public IRepository<GoodbyeChannelDTO> {
  public:
	[[nodiscard]] bool create(const GoodbyeChannelDTO& object) override;

	[[nodiscard]] bool remove(size_t guild_id) override;

	[[nodiscard]] GoodbyeChannelDTO get(size_t guild_id) override;

	[[nodiscard]] bool update(const GoodbyeChannelDTO& object) override;
};
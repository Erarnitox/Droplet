#pragma once

#include <vector>
#include "BlacklistDTO.hpp"
#include "IRepository.hpp"

class BlacklistRepository : public IRepository<BlacklistDTO> {
  public:
	[[nodiscard]] bool create(const BlacklistDTO& object) noexcept override;

	[[nodiscard]] bool remove(size_t id) noexcept override;

	[[nodiscard]] BlacklistDTO get(size_t id) noexcept override;

	[[nodiscard]] bool update(const BlacklistDTO& object) noexcept override;

	[[nodiscard]] std::vector<BlacklistDTO> getAll() noexcept;
};
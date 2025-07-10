#pragma once

#include <IRepository.hpp>

#include "ResourceDTO.hpp"

class ResourceRepository : public IRepository<ResourceDTO> {
  public:
	[[nodiscard]] bool create(const ResourceDTO& object) noexcept override;

	[[nodiscard]] bool remove(size_t id) noexcept override;

	[[nodiscard]] ResourceDTO get(size_t id) noexcept override;

	[[nodiscard]] std::vector<ResourceDTO> get(const std::string& category) noexcept;

	[[nodiscard]] bool update(const ResourceDTO& object) noexcept override;
};
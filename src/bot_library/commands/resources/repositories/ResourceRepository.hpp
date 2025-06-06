#pragma once

#include <IRepository.hpp>

#include "ResourceDTO.hpp"

class ResourceRepository : public IRepository<ResourceDTO> {
  public:
	[[nodiscard]] bool create(const ResourceDTO& object) override;

	[[nodiscard]] bool remove(size_t id) override;

	[[nodiscard]] ResourceDTO get(size_t id) override;

	[[nodiscard]] std::vector<ResourceDTO> get(const std::string& category);

	[[nodiscard]] bool update(const ResourceDTO& object) override;
};
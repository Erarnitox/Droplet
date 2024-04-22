#pragma once

#include <IRepository.hpp>

#include "ProductDTO.hpp"

class ProductRepository : public IRepository<ProductDTO> {
  public:
	[[nodiscard]] auto create(const ProductDTO& object) -> bool override;

	[[nodiscard]] auto remove(size_t id) -> bool override;

	[[nodiscard]] auto get(size_t id) -> ProductDTO override;

	[[nodiscard]] auto update(const ProductDTO& object) -> bool override;
};
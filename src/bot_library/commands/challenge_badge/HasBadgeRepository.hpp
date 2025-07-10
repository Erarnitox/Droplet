#pragma once

#include <vector>

class HasBadgeRepository {
  public:
	[[nodiscard]] bool create(size_t user_id, size_t message_id) noexcept;

	[[nodiscard]] bool remove(size_t user_id, size_t message_id = 0) noexcept;
	[[nodiscard]] bool removeBadge(size_t message_id) noexcept;

	[[nodiscard]] std::vector<size_t> get(size_t user_id) noexcept;
};
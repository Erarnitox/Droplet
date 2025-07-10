#pragma once

#include "IRepository.hpp"
#include "NotificationDTO.hpp"

class NotificationRepository : public IRepository<NotificationDTO> {
  public:
	[[nodiscard]] bool create(const NotificationDTO& object) noexcept override;

	[[nodiscard]] bool remove(size_t id) noexcept override;

	[[nodiscard]] NotificationDTO get(size_t id) noexcept override;

	[[nodiscard]] bool update(const NotificationDTO& object) noexcept override;

	[[nodiscard]] std::vector<NotificationDTO> getAll() noexcept;
};
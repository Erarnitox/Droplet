#pragma once

#include "IRepository.hpp"
#include "NotificationDTO.hpp"

class NotificationRepository : public IRepository<NotificationDTO> {
  public:
	[[nodiscard]] auto create(const NotificationDTO& object) -> bool override;

	[[nodiscard]] auto remove(size_t id) -> bool override;

	[[nodiscard]] auto get(size_t id) -> NotificationDTO override;

	[[nodiscard]] auto update(const NotificationDTO& object) -> bool override;

	[[nodiscard]] auto getAll() -> std::vector<NotificationDTO>;
};
#pragma once

#include <IRepository.hpp>

#include "ReactionRoleDTO.hpp"

class ReactionRoleRepository : public IRepository<ReactionRoleDTO> {
  public:
	[[nodiscard]] bool create(const ReactionRoleDTO& object) noexcept override;

	[[nodiscard]] bool remove(size_t message_id) noexcept override;

	[[nodiscard]] ReactionRoleDTO get(size_t message_id) noexcept override;

	[[nodiscard]] ReactionRoleDTO get(size_t message_id, const std::string& emoji) noexcept;

	[[nodiscard]] bool update(const ReactionRoleDTO& object) noexcept override;
};
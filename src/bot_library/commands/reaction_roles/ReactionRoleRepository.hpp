#pragma once

#include <IRepository.hpp>

#include "ReactionRoleDTO.hpp"

class ReactionRoleRepository : public IRepository<ReactionRoleDTO> {
  public:
	[[nodiscard]] bool create(const ReactionRoleDTO& object) override;

	[[nodiscard]] bool remove(size_t message_id) override;

	[[nodiscard]] ReactionRoleDTO get(size_t message_id) override;

	[[nodiscard]] ReactionRoleDTO get(size_t message_id, const std::string& emoji);

	[[nodiscard]] bool update(const ReactionRoleDTO& object) override;
};
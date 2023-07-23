#pragma once

#include <IRepository.hpp>
#include "ChallengeRoleDTO.hpp"

class ChallengeRoleRepository : public IRepository<ChallengeRoleDTO> {
    public:
        void create(const ChallengeRoleDTO& object) override;
        void remove(size_t messageId) override;
        ChallengeRoleDTO get(size_t messageId) override;
        void update(const ChallengeRoleDTO& object) override;
};
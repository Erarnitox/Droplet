#pragma once

#include <IRepository>
#include "ChallengeRoleDTO.hpp"

class ChallengeRoleRepository : public IRepository<ChallengeRoleDTO> {
    public:
        size_t create(const ChallengeRoleDTO& object) override;
        bool remove(size_t id) override;
        ChallengeRoleDTO get(size_t id) override;
        bool update(size_t id, const ChallengeRoleDTO& object) override;
};
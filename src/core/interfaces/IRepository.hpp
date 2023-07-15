#pragma once

#include <cstddef>
template <typename DTO>
class IRepository {
    public:
        virtual void    create(const DTO& object)               = 0;
        virtual void    remove(size_t id)                       = 0;
        [[nodiscard]]
        virtual DTO     get(size_t id)                          = 0;
        virtual void    update(size_t id, const DTO& object)    = 0;
};
#pragma once

#include <cstddef>
template <typename DTO>
class IRepository {
    public:
        virtual void    create(const DTO& object)               = 0;
        virtual void    remove(size_t index)                    = 0;
        [[nodiscard]]
        virtual DTO     get(size_t index)                       = 0;
        virtual void    update(const DTO& object)               = 0;
};
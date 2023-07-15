#pragma once

template <typename DTO>
class IRepository {
    public:
        size_t create(const DTO& object) override;
        bool remove(size_t id) override;
        DTO get(size_t id) override;
        bool update(size_t id, const DTO& object) override;
};
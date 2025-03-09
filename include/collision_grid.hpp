#ifndef COLLISION_GRID_HPP
#define COLLISION_GRID_HPP

#include <vector>

#include "object.hpp"

struct collision_grid_t {
    uint32_t width;
    uint32_t height;
    std::vector<std::vector<object_t*>> grid{};
    std::vector<object_t*> empty{};

    collision_grid_t(uint32_t width, uint32_t height);
    void add_object(uint32_t x, uint32_t y, object_t* object);
    const std::vector<object_t*>& get_objects(uint32_t x, uint32_t y) const;
    void clear();
    void check_collision_cells(const std::vector<object_t*>& cell1, const std::vector<object_t*>& cell2);
};

#endif // COLLISION_GRID_HPP 
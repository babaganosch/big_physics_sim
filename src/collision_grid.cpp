#include "collision_grid.hpp"

collision_grid_t::collision_grid_t(uint32_t width, uint32_t height) : width(width), height(height) {
    grid.resize(width * height);
}

void collision_grid_t::add_object(uint32_t x, uint32_t y, object_t* object) {
    if (x >= width || y >= height) return;
    grid[x + y * width].push_back(object);
}

const std::vector<object_t*>& collision_grid_t::get_objects(uint32_t x, uint32_t y) const {
    if (x >= width || y >= height) { return empty; }
    return grid[x + y * width];
}

void collision_grid_t::clear() {
    for (auto& row : grid) {
        row.clear();
    }
}

void collision_grid_t::check_collision_cells(const std::vector<object_t*>& cell1, const std::vector<object_t*>& cell2) {
    for (auto& object1 : cell1) {
        if (!object1) continue;

        for (auto& object2 : cell2) {
            if (!object2) continue;
            if (object1 == object2) continue;

            object1->handle_collision(*object2);
        }
    }
} 
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

void collision_grid_t::handle_collisions() {

    handle_collision_chunk(0, 0, width, height);
    
}

void collision_grid_t::handle_collision_chunk(int16_t sx, int16_t sy, int16_t ex, int16_t ey) {
    // Check for object collisions
    for (int16_t i = sx; i < ex; ++i) {
        for (int16_t j = sy; j < ey; ++j) {

            const auto& cell = get_objects(i, j);

            // check surrounding cells
            for (int16_t dx = -1; dx <= 1; ++dx) {
                for (int16_t dy = -1; dy <= 1; ++dy) {

                    const auto& other_cell = get_objects(i + dx, j + dy);
                    check_collision_cells(cell, other_cell);
                }
            }
        }
    }
}   
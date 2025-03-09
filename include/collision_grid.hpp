#ifndef COLLISION_GRID_HPP
#define COLLISION_GRID_HPP

#include <vector>

#include "object.hpp"

struct collision_grid_t {
    uint32_t width;
    uint32_t height;
    
    static constexpr size_t MAX_OBJECTS_PER_CELL = 32;
    struct Cell {
        object_t* objects[MAX_OBJECTS_PER_CELL];
        uint16_t count = 0;
    };
    
    std::vector<Cell> grid;

    collision_grid_t(uint32_t width, uint32_t height);
    void clear();
    void handle_collisions();
    void add_object(uint32_t x, uint32_t y, object_t* object);
    void handle_collision_chunk(uint32_t sx, uint32_t sy, uint32_t ex, uint32_t ey);

private:
    inline Cell& get_cell(uint32_t x, uint32_t y) {
        return grid[y * width + x];
    }
    
    inline const Cell& get_cell(uint32_t x, uint32_t y) const {
        return grid[y * width + x];
    }

    void check_collision_cells(const Cell& cell1, const Cell& cell2);
};

#endif // COLLISION_GRID_HPP 
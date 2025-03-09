#include "collision_grid.hpp"

#include <pthread.h>

collision_grid_t::collision_grid_t(uint32_t width, uint32_t height) : width(width), height(height) {
    grid.resize(width * height);
}

void collision_grid_t::add_object(uint32_t x, uint32_t y, object_t* object) {
    if (x >= width || y >= height) return;
    Cell& cell = get_cell(x, y);
    if (cell.count < MAX_OBJECTS_PER_CELL) {
        cell.objects[cell.count++] = object;
    } else {
        printf("PANIC! Cell at %d, %d is full!\n", x, y);
    }
}

void collision_grid_t::clear() {
    for (auto& cell : grid) {
        cell.count = 0;
    }
}

void collision_grid_t::check_collision_cells(const Cell& cell1, const Cell& cell2) {
    if (cell1.count == 0 || cell2.count == 0) return;

    const uint16_t count1 = cell1.count;
    const uint16_t count2 = cell2.count;
    object_t* const* objects1 = cell1.objects;
    object_t* const* objects2 = cell2.objects;

    for (uint16_t i = 0; i < count1; ++i) {
        object_t* obj1 = objects1[i];
        if (!obj1) continue;

        for (uint16_t j = 0; j < count2; ++j) {
            object_t* obj2 = objects2[j];
            if (!obj2) continue;

            if (obj1 != obj2) obj1->handle_collision(*obj2);
        }
    }
}

struct chunk_params {
    uint32_t sx;
    uint32_t sy;
    uint32_t ex;
    uint32_t ey;
    collision_grid_t* grid;
};

void* handle_collision_chunk_thread(void* arg) {
    chunk_params* params = (chunk_params*)arg;
    params->grid->handle_collision_chunk(params->sx, params->sy, params->ex, params->ey);
    return nullptr;
}

void collision_grid_t::handle_collisions() {
    const uint32_t CHUNK_WIDTH = width / 8;
    chunk_params params[8] = {
        {CHUNK_WIDTH * 0, 0, CHUNK_WIDTH * 1, height, this},
        {CHUNK_WIDTH * 1, 0, CHUNK_WIDTH * 2, height, this},
        {CHUNK_WIDTH * 2, 0, CHUNK_WIDTH * 3, height, this},
        {CHUNK_WIDTH * 3, 0, CHUNK_WIDTH * 4, height, this},
        {CHUNK_WIDTH * 4, 0, CHUNK_WIDTH * 5, height, this},
        {CHUNK_WIDTH * 5, 0, CHUNK_WIDTH * 6, height, this},
        {CHUNK_WIDTH * 6, 0, CHUNK_WIDTH * 7, height, this},
        {CHUNK_WIDTH * 7, 0, width,           height, this}
    };

    pthread_t threads[8];
    for (int i = 0; i < 8; ++i) {
        pthread_create(&threads[i], NULL, handle_collision_chunk_thread, &params[i]);
    }

    for (int i = 0; i < 8; ++i) {
        pthread_join(threads[i], NULL);
    }
}

void collision_grid_t::handle_collision_chunk(uint32_t sx, uint32_t sy, uint32_t ex, uint32_t ey) {

    // Loop through all cells in the chunk
    for (int16_t i = sx; i < ex; ++i) {
        for (int16_t j = sy; j < ey; ++j) {

            const auto& cell = get_cell(i, j);

            // Check surrounding cells
            for (int16_t dx = -1; dx <= 1; ++dx) {
                for (int16_t dy = -1; dy <= 1; ++dy) {
                
                    // Outside of grid, skip!
                    if (i + dx < 0 || i + dx >= width || j + dy < 0 || j + dy >= height) continue;

                    const auto& other_cell = get_cell(i + dx, j + dy);
                    check_collision_cells(cell, other_cell);
                }
            }
        }
    }
}   
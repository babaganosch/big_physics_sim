#include "collision_grid.hpp"

#include <pthread.h>

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

struct chunk_params {
    int16_t sx;
    int16_t sy;
    int16_t ex;
    int16_t ey;
    collision_grid_t* grid;
};

void* handle_collision_chunk_thread(void* arg) {
    chunk_params* params = (chunk_params*)arg;
    params->grid->handle_collision_chunk(params->sx, params->sy, params->ex, params->ey);
    return nullptr;
}

void collision_grid_t::handle_collisions() {

    const int16_t CHUNK_WIDTH = (int16_t)(width / 8);
    chunk_params params1 = {0, 0, CHUNK_WIDTH, (int16_t)height, this};
    chunk_params params2 = {(int16_t)(CHUNK_WIDTH * 1), 0, (int16_t)(CHUNK_WIDTH * 2), (int16_t)height, this};
    chunk_params params3 = {(int16_t)(CHUNK_WIDTH * 2), 0, (int16_t)(CHUNK_WIDTH * 3), (int16_t)height, this};
    chunk_params params4 = {(int16_t)(CHUNK_WIDTH * 3), 0, (int16_t)(CHUNK_WIDTH * 4), (int16_t)height, this};
    chunk_params params5 = {(int16_t)(CHUNK_WIDTH * 4), 0, (int16_t)(CHUNK_WIDTH * 5), (int16_t)height, this};
    chunk_params params6 = {(int16_t)(CHUNK_WIDTH * 5), 0, (int16_t)(CHUNK_WIDTH * 6), (int16_t)height, this};
    chunk_params params7 = {(int16_t)(CHUNK_WIDTH * 6), 0, (int16_t)(CHUNK_WIDTH * 7), (int16_t)height, this};
    chunk_params params8 = {(int16_t)(CHUNK_WIDTH * 7), 0, (int16_t)width, (int16_t)height, this};

    pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7, thread8;
    pthread_create(&thread1, NULL, handle_collision_chunk_thread, (void*)&params1);
    pthread_create(&thread2, NULL, handle_collision_chunk_thread, (void*)&params2);
    pthread_create(&thread3, NULL, handle_collision_chunk_thread, (void*)&params3);
    pthread_create(&thread4, NULL, handle_collision_chunk_thread, (void*)&params4);
    pthread_create(&thread5, NULL, handle_collision_chunk_thread, (void*)&params5);
    pthread_create(&thread6, NULL, handle_collision_chunk_thread, (void*)&params6);
    pthread_create(&thread7, NULL, handle_collision_chunk_thread, (void*)&params7);
    pthread_create(&thread8, NULL, handle_collision_chunk_thread, (void*)&params8);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);
    pthread_join(thread6, NULL);
    pthread_join(thread7, NULL);
    pthread_join(thread8, NULL);
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
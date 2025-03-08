#include "world.hpp"
#include <cmath>
#include <algorithm>

#define CELL_SIZE 24

// Helper function to check and resolve collisions between two objects
static void checkCollision(object_t& object, object_t& other) {
    if (object.id == other.id) return;

    const float dx = other.position.x - object.position.x;
    const float dy = other.position.y - object.position.y;
    const float min_distance = object.radius + other.radius;

    const float distance_sq = dx * dx + dy * dy;
    if (distance_sq < min_distance * min_distance) {
        const float distance = std::sqrt(distance_sq);
        const sf::Vector2f normal(dx / distance, dy / distance);
        const sf::Vector2f correction = normal * 0.5f * (min_distance - distance);
        object.position -= correction;
        other.position += correction;
    }
}

world_t::world_t(uint32_t width, uint32_t height, float gravity)
{
    this->width = width;
    this->height = height;
    this->gravity = gravity;

    // Load sprite used by all objects
    if (!texture.loadFromFile("assets/circle.png"))
        throw std::runtime_error("Failed to load circle sprite");

    // Initialize grid with proper dimensions
    const uint32_t grid_width = (width + CELL_SIZE - 1) / CELL_SIZE;
    const uint32_t grid_height = (height + CELL_SIZE - 1) / CELL_SIZE;
    
    printf("grid_width: %d, grid_height: %d\n", grid_width, grid_height);

    grid.resize(grid_width);
    for (auto& row : grid) {
        row.resize(grid_height);
    }
}

void world_t::update(float delta_time)
{
    constexpr uint32_t SUB_STEPS = 4;
    const float sub_delta_time = delta_time / SUB_STEPS;

    for (uint32_t step = 0; step < SUB_STEPS; ++step) {
        // Clear grid
        for (auto& row : grid) {
            for (auto& cell : row) {
                cell.clear();
            }
        }

        // Update objects
        for (object_t& object : objects) {

            // Apply gravity
            object.acceleration.y += gravity;

            // Collision against walls
            object.position.x = std::min(std::max(object.position.x, (float)object.radius), (float)width - object.radius);
            object.position.y = std::min(std::max(object.position.y, (float)object.radius), (float)height - object.radius);

            const int32_t x = static_cast<int32_t>(object.position.x) / CELL_SIZE;
            const int32_t y = static_cast<int32_t>(object.position.y) / CELL_SIZE;
            
            // Check bounds before accessing grid
            if (x >= 0 && x < static_cast<int32_t>(grid.size()) && 
                y >= 0 && y < static_cast<int32_t>(grid[0].size())) {
                grid[x][y].push_back(&object);
            }
        }

        // Then check collisions between objects
        for (size_t i = 0; i < grid.size(); ++i) {
            for (size_t j = 0; j < grid[i].size(); ++j) {
                auto& cell = grid[i][j];

                // For each object in this cell
                for (size_t k = 0; k < cell.size(); ++k) {
                    object_t& object = *cell[k];

                    // Check collisions with objects in current cell (starting from k+1 to avoid duplicates)
                    for (size_t l = k + 1; l < cell.size(); ++l) {
                        checkCollision(object, *cell[l]);
                    }

                    // Check collisions with objects in neighboring cells
                    // We check the current cell's objects against objects in the 8 surrounding cells
                    // This ensures we catch all possible collisions between objects in adjacent cells
                    // NOTE: This is probably something that can be optimized quite a bit..
                    for (int32_t dy = -1; dy <= 1; ++dy) {
                        for (int32_t dx = -1; dx <= 1; ++dx) {
                            if (dx == 0 && dy == 0) continue; // We checked this cell earlier ;)

                            const int32_t check_i = static_cast<int32_t>(i) + dx;
                            const int32_t check_j = static_cast<int32_t>(j) + dy;

                            // Skip if out of bounds
                            if (check_i < 0 || check_i >= static_cast<int32_t>(grid.size()) ||
                                check_j < 0 || check_j >= static_cast<int32_t>(grid[0].size())) {
                                continue;
                            }

                            // Check collisions with all objects in the neighboring cell
                            auto& other_cell = grid[check_i][check_j];
                            for (object_t* other : other_cell) {
                                checkCollision(object, *other);
                            }
                        }
                    }
                }
            }
        }

        // Update object positions
        for (object_t& object : objects) {
            object.update(sub_delta_time);
        }
    }

    // Finally update vertex array
    for (size_t i = 0; i < objects.size();  ++i) {
        const size_t obj_index = i * 4;
        va[obj_index + 0] = objects[i].vertices[0];
        va[obj_index + 1] = objects[i].vertices[1];
        va[obj_index + 2] = objects[i].vertices[2];
        va[obj_index + 3] = objects[i].vertices[3];
    }
}

void world_t::render(sf::RenderWindow& window)
{
    window.clear();
    window.draw(va, &texture);
}

void world_t::addObject(const object_t& object)
{
    objects.push_back(object);
    va.resize(objects.size() * 4);
}
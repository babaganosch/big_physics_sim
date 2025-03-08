#include <cmath>
#include <algorithm>

#include "world.hpp"

static uint16_t cell_size = 128;
world_t::world_t(uint32_t width, uint32_t height, float gravity, float min_radius, float max_radius) {
    this->width = width;
    this->height = height;
    this->gravity = gravity;
    this->min_radius = min_radius;  
    this->max_radius = max_radius;

    cell_size = std::max(min_radius, max_radius) * 2.0f;

    // Load sprite used by all objects
    if (!texture.loadFromFile("assets/circle.png"))
        throw std::runtime_error("Failed to load circle sprite");

    // Initialize grid with proper dimensions
    const uint16_t grid_width = (width + cell_size - 1) / cell_size;
    const uint16_t grid_height = (height + cell_size - 1) / cell_size;

    this->grid = collision_grid_t(grid_width, grid_height);
    
    printf("Collision grid width: %d, height: %d\n", grid_width, grid_height);

}

void world_t::update(float delta_time)
{
    constexpr uint16_t SUB_STEPS = 6;
    const float sub_delta_time = delta_time / SUB_STEPS;

    for (uint16_t step = 0; step < SUB_STEPS; ++step) {
        // Clear grid
        grid.clear();

        // Update objects
        for (object_t& object : objects) {

            // Apply gravity
            object.acceleration.y += gravity;

            // Collision against walls (clamp them back in bounds)
            object.position.x = std::min(std::max(object.position.x, (float)object.radius), (float)width - object.radius);
            object.position.y = std::min(std::max(object.position.y, (float)object.radius), (float)height - object.radius);

            const int16_t x = static_cast<int16_t>(object.position.x) / cell_size;
            const int16_t y = static_cast<int16_t>(object.position.y) / cell_size;
            
            grid.add_object(x, y, &object);
        }

        // Check for object collisions
        grid.handle_collisions();

        // Update objects
        for (size_t i = 0; i < objects.size();  ++i) {
            objects[i].update(sub_delta_time);

            // Update vertex array on last substep
            if (step == SUB_STEPS - 1) {
                const size_t obj_index = i * 4;
                va[obj_index + 0] = objects[i].vertices[0];
                va[obj_index + 1] = objects[i].vertices[1];
                va[obj_index + 2] = objects[i].vertices[2];
                va[obj_index + 3] = objects[i].vertices[3];
            }
        }
    }
}

void world_t::render(sf::RenderWindow& window)
{
    window.clear();
    window.draw(va, &texture);
}

void world_t::spawn_object()
{
    spawn_object(rand() % width, rand() % height);
}

void world_t::spawn_object( uint32_t x, uint32_t y )
{
    float radius = min_radius + (((rand() % 100) / 100.0f) * (max_radius - min_radius));
    x = std::min(std::max((float)x, (float)radius), (float)width - radius);
    y = std::min(std::max((float)y, (float)radius), (float)height - radius);
    object_t object(
        radius,
        sf::Vector2f(x, y),
        sf::Color(rand() % 256, rand() % 256, rand() % 256)
    );
    objects.push_back(object);
    va.resize(objects.size() * 4);
}
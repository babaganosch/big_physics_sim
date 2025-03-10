#include <cmath>
#include <algorithm>
#include <pthread.h>
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

struct update_objects_params {
    uint32_t si;
    uint32_t ei;
    float sub_delta_time;
    bool last_step;
    world_t* world;
};

void* post_update_objects(void* arg)
{
    update_objects_params* params = (update_objects_params*)arg;
    
    uint32_t start_index = params->si;
    uint32_t end_index = params->ei;
    float sub_delta_time = params->sub_delta_time;
    world_t* world = params->world;

    for (uint32_t i = start_index; i < end_index; ++i) {
        world->objects[i].update(sub_delta_time);

        if (params->last_step) {
            world->objects[i].update_vertices();
            const size_t obj_index = i * 4;
            world->va[obj_index + 0] = world->objects[i].vertices[0];
            world->va[obj_index + 1] = world->objects[i].vertices[1];
            world->va[obj_index + 2] = world->objects[i].vertices[2];
            world->va[obj_index + 3] = world->objects[i].vertices[3];
        }
    }

    return nullptr;
}

void* pre_update_objects(void* arg)
{
    update_objects_params* params = (update_objects_params*)arg;
    uint32_t start_index = params->si;
    uint32_t end_index = params->ei;
    float sub_delta_time = params->sub_delta_time;
    world_t* world = params->world;

    for (uint32_t i = start_index; i < end_index; ++i) {
        object_t& object = world->objects[i];

        // Apply gravity
        object.acceleration.y += world->gravity;

        // Collision against walls (clamp them back in bounds)
        object.position.x = std::min(std::max(object.position.x, (float)object.radius), (float)world->width - object.radius);
        object.position.y = std::min(std::max(object.position.y, (float)object.radius), (float)world->height - object.radius);

        const int16_t x = static_cast<int16_t>(object.position.x) / cell_size;
        const int16_t y = static_cast<int16_t>(object.position.y) / cell_size;
        
        world->grid.add_object(x, y, &object);
    }

    return nullptr;
}


void world_t::update(float delta_time)
{
    constexpr uint16_t SUB_STEPS = 6;
    const float sub_delta_time = delta_time / SUB_STEPS;

    pthread_t threads[8];
    uint32_t step_size = objects.size() / 8;

    for (uint16_t step = 0; step < SUB_STEPS; ++step) {

        const bool last_step = step == SUB_STEPS - 1;

        update_objects_params params[8] = {
            {0, step_size, sub_delta_time, last_step, this},
            {step_size, step_size * 2, sub_delta_time, last_step, this},
            {step_size * 2, step_size * 3, sub_delta_time, last_step, this},
            {step_size * 3, step_size * 4, sub_delta_time, last_step, this},
            {step_size * 4, step_size * 5, sub_delta_time, last_step, this},
            {step_size * 5, step_size * 6, sub_delta_time, last_step, this},
            {step_size * 6, step_size * 7, sub_delta_time, last_step, this},
            {step_size * 7, (uint32_t)objects.size(), sub_delta_time, last_step, this}
        };

        // Clear grid
        grid.clear();

        // Add objects to collision grid, apply gravity and clamp to walls
        for (int i = 0; i < 8; ++i) {
            pthread_create(&threads[i], NULL, pre_update_objects, &params[i]);
        }

        for (int i = 0; i < 8; ++i) {
            pthread_join(threads[i], NULL);
        }

        // Check for object collisions
        grid.handle_collisions();

        // Finally, update objects attributes and vertices
        for (int i = 0; i < 8; ++i) {
            pthread_create(&threads[i], NULL, post_update_objects, &params[i]);
        }

        for (int i = 0; i < 8; ++i) {
            pthread_join(threads[i], NULL);
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
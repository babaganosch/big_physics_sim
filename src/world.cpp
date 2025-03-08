#include "world.hpp"
#include <cmath>
#include <algorithm>

world_t::world_t(uint32_t width, uint32_t height, float gravity)
{
    this->width = width;
    this->height = height;
    this->gravity = gravity;

    // Load sprite used by all objects
    if (!texture.loadFromFile("assets/circle.png"))
        throw std::runtime_error("Failed to load circle sprite");
}

void world_t::update(float delta_time)
{
    // Update objects
    for (size_t i = 0; i < objects.size(); ++i) {
        object_t& object = objects[i];

        // Apply gravity
        object.acceleration.y += gravity;

        // Collision against walls
        object.position.x = std::min(std::max(object.position.x, (float)object.radius), (float)width - object.radius);
        object.position.y = std::min(std::max(object.position.y, (float)object.radius), (float)height - object.radius);

        // Collision against other objects
        for (size_t j = i+1; j < objects.size(); ++j) {
            object_t& other = objects[j];

            const sf::Vector2f delta = other.position - object.position;
            const float distance = sqrt((delta.x * delta.x) + (delta.y * delta.y));
            const float min_distance = object.radius + other.radius;

            if (distance < min_distance) {
                // Collision detected
                const sf::Vector2f normal = delta / distance;
                const sf::Vector2f correction = normal * 0.5f * (min_distance - distance);
                object.position -= correction;
                other.position += correction;
            }
        }

        // Update object position
        object.update(delta_time);
        
        // Update vertex array
        const size_t obj_index = i * 4;
        va[obj_index + 0] = object.vertices[0];
        va[obj_index + 1] = object.vertices[1];
        va[obj_index + 2] = object.vertices[2];
        va[obj_index + 3] = object.vertices[3];
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
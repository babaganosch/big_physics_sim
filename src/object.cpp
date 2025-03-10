#include <cmath>

#include "object.hpp"

object_t::object_t( float radius, sf::Vector2f position, sf::Color color )
{
    this->radius = radius;
    this->position = position;
    this->position_prev = position;

    // Initialize vertices
    vertices[0].position = position + sf::Vector2f(-radius, -radius);
    vertices[1].position = position + sf::Vector2f(radius, -radius);
    vertices[2].position = position + sf::Vector2f(radius, radius);
    vertices[3].position = position + sf::Vector2f(-radius, radius);

    vertices[0].texCoords = sf::Vector2f(0, 0);
    vertices[1].texCoords = sf::Vector2f(SPRITE_WIDTH, 0);
    vertices[2].texCoords = sf::Vector2f(SPRITE_WIDTH, SPRITE_HEIGHT);
    vertices[3].texCoords = sf::Vector2f(0, SPRITE_HEIGHT);

    vertices[0].color = color;
    vertices[1].color = color;
    vertices[2].color = color;
    vertices[3].color = color;
}

void object_t::update( float delta_time )
{
    sf::Vector2f velocity = (position - position_prev);
    position_prev = position;

    position = position + velocity + acceleration * delta_time * delta_time;
    acceleration = sf::Vector2f(0, 0);
}

void object_t::update_vertices()
{
    vertices[0].position = position + sf::Vector2f(-radius, -radius);
    vertices[1].position = position + sf::Vector2f(radius, -radius);
    vertices[2].position = position + sf::Vector2f(radius, radius);
    vertices[3].position = position + sf::Vector2f(-radius, radius);
}

void object_t::handle_collision( object_t& other )
{
    const float dx = other.position.x - position.x;
    const float dy = other.position.y - position.y;
    const float min_distance = radius + other.radius;

    const float distance_sq = dx * dx + dy * dy;
    if (distance_sq < min_distance * min_distance) {
        const float distance = std::sqrt(distance_sq);
        
        if (distance < 0.000001f) {
            const sf::Vector2f push = sf::Vector2f(min_distance * 0.5f, min_distance * 0.5f);
            other.position += push;
            position -= push;
            return;
        }

        const sf::Vector2f normal = sf::Vector2f(dx / distance, dy / distance);
        const sf::Vector2f correction = normal * 0.5f * (min_distance - distance);
        other.position += correction;
        position -= correction;
    }
}
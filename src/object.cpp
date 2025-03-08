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

    vertices[0].position = position + sf::Vector2f(-radius, -radius);
    vertices[1].position = position + sf::Vector2f(radius, -radius);
    vertices[2].position = position + sf::Vector2f(radius, radius);
    vertices[3].position = position + sf::Vector2f(-radius, radius);
}

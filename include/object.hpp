#include <cstdint>
#include <SFML/Graphics.hpp>

#ifndef OBJECT_HPP
#define OBJECT_HPP

#define SPRITE_WIDTH  256
#define SPRITE_HEIGHT 256

struct object_t 
{
    uint8_t radius;
    sf::Vector2f position;
    sf::Vector2f velocity;

    // Quad vertices
    //  0---1
    //  | / |
    //  3---2
    sf::Vertex vertices[4];

    object_t( float radius, sf::Vector2f position, sf::Vector2f velocity, sf::Color color );
    void update( );
};

#endif /* OBJECT_HPP */
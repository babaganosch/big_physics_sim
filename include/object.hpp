#include <cstdint>
#include <SFML/Graphics.hpp>

#ifndef OBJECT_HPP
#define OBJECT_HPP

#define SPRITE_WIDTH  256
#define SPRITE_HEIGHT 256

struct object_t 
{
    float radius;
    sf::Vector2f position{0, 0};
    sf::Vector2f position_prev{0, 0};
    sf::Vector2f acceleration{0, 0};

    // Quad vertices
    //  0---1
    //  | / |
    //  3---2
    sf::Vertex vertices[4];

    object_t( float radius, sf::Vector2f position, sf::Color color );
    void update( float delta_time );
};

#endif /* OBJECT_HPP */
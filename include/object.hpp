#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <cstdint>
#include <SFML/Graphics.hpp>

#define SPRITE_WIDTH  256
#define SPRITE_HEIGHT 256

static uint32_t next_object_id = 0;
struct object_t 
{
    uint32_t id{next_object_id++};
    uint8_t radius;
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
    void handle_collision( object_t& other );
};

#endif /* OBJECT_HPP */
#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <SFML/Graphics.hpp>

#include "object.hpp"
#include "collision_grid.hpp"

struct world_t 
{
    uint32_t width;
    uint32_t height;
    float    gravity;
    float    min_radius;
    float    max_radius;

    sf::Texture texture;
    sf::VertexArray va{sf::Quads, 0};
    std::vector<object_t> objects{};
    collision_grid_t grid{0, 0};

    world_t(uint32_t width, uint32_t height, float gravity, float min_radius, float max_radius);

    void update(float delta_time);
    void render(sf::RenderWindow& window);
    void spawn_object();
};

#endif /* WORLD_HPP */

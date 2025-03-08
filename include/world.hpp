#ifndef WORLD_HPP
#define WORLD_HPP

#include "object.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

struct world_t 
{
    uint32_t width;
    uint32_t height;
    float    gravity;

    sf::Texture texture;
    sf::VertexArray va{sf::Quads, 0};
    std::vector<object_t> objects{};

    world_t(uint32_t width, uint32_t height, sf::Texture& texture, float gravity) : width(width), height(height), texture(texture), gravity(gravity) {}

    void update();
    void render(sf::RenderWindow& window);
    void addObject(const object_t& object);
};

#endif /* WORLD_HPP */

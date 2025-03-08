#include "world.hpp"

void world_t::update()
{
    // Update objects
    for (size_t i = 0; i < objects.size(); ++i) {
        object_t& object = objects[i];

        object.velocity.y += gravity;

        object.update();

        if (object.position.x < 0 || object.position.x > width) {
            object.velocity.x = -object.velocity.x;
        }
        if (object.position.y < 0 || object.position.y > height) {
            object.velocity.y = -object.velocity.y;
        }

        const size_t obj_index = i * 4;
        va[obj_index + 0] = object.vertices[0];
        va[obj_index + 1] = object.vertices[1];
        va[obj_index + 2] = object.vertices[2];
        va[obj_index + 3] = object.vertices[3];
    }
}

void world_t::render(sf::RenderWindow& window)
{
    window.draw(va, &texture);
}

void world_t::addObject(const object_t& object)
{
    objects.push_back(object);
    va.resize(objects.size() * 4);
}
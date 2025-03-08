#include <SFML/Graphics.hpp>
#include "world.hpp"
#include <iomanip>
#include <sstream>

constexpr uint32_t WINDOW_WIDTH = 1200;
constexpr uint32_t WINDOW_HEIGHT = 600;
constexpr uint32_t NUM_OBJECTS = 1000;

int main() 
{
    // Create a window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Big Physics Simulation");
    window.setFramerateLimit(60);
        
    //// GUI
    // Text
    sf::Font font;
    if (!font.loadFromFile("assets/source-code-pro.regular.ttf"))
        return EXIT_FAILURE;
    sf::Text text("", font, 16);
    text.setPosition(10, 10);
    // Bg
    sf::RectangleShape rect(sf::Vector2f(225, 160));
    rect.setFillColor(sf::Color(0, 0, 0, 128));

    // Create a world with randomly positioned objects, move to proper spawn method later..
    world_t world(WINDOW_WIDTH, WINDOW_HEIGHT, 200.f);
    for (uint32_t i = 0; i < NUM_OBJECTS; i++) {
        world.addObject(object_t(
            1.0f + (((rand() % 100) / 100.0f) * 10.0f),                                          // radius
            sf::Vector2f(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT),                         // position
            sf::Color(rand() % 256, rand() % 256, rand() % 256)                                  // color
        ));
    }

    // Timing
    sf::Clock clock;
    sf::Clock frame_clock;
    sf::Time last_update_time;
    sf::Time last_render_time;
    sf::Time last_frame_time;

    // Main game loop
    while (window.isOpen()) 
    {
        last_frame_time = frame_clock.getElapsedTime();
        frame_clock.restart();
        float delta_time = last_frame_time.asSeconds();

        /////////////// Handle SFML events, close game etc..
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        /////////////// Update the world
        clock.restart();
        world.update(delta_time);
        last_update_time = clock.getElapsedTime();
        
        /////////////// Render the world
        clock.restart();
        world.render(window);
        last_render_time = clock.getElapsedTime();

        /////////////// GUI
        window.draw(rect);

        std::stringstream ss;
        ss << std::fixed << std::setprecision(3);
        float total_time = (last_update_time + last_render_time).asMicroseconds() / 1000.0f;
        float frame_time = delta_time * 1000.0f;
        
        ss << "objects:   " << world.objects.size() << "\n"
           << "update:    " << last_update_time.asMicroseconds() / 1000.0f << "ms\n"
           << "render:    " << last_render_time.asMicroseconds() / 1000.0f << "ms\n"
           << "\n"
           << "duty     / idle\n"
           << total_time << "ms / " << frame_time - total_time << "ms\n"
           << "fps: " << (frame_time > 0 ? 1000.0f / frame_time : 0.0f);

        text.setString(ss.str());
        window.draw(text);

        // Swap framebuffers
        window.display();
    }

    return EXIT_SUCCESS;
} 
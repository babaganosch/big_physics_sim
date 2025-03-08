#include <SFML/Graphics.hpp>
#include "world.hpp"
#include <iomanip>
#include <sstream>

constexpr uint32_t WINDOW_WIDTH = 1600;
constexpr uint32_t WINDOW_HEIGHT = 900;
constexpr uint32_t NUM_OBJECTS = 100000;

int main() 
{
    // Create a window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Big Physics Simulation", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    // Load sprite used by all objects
    sf::Texture texture;
    if (!texture.loadFromFile("assets/circle.png"))
        return EXIT_FAILURE;
        
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
    world_t world(WINDOW_WIDTH, WINDOW_HEIGHT, texture, 0.f);
    for (uint32_t i = 0; i < NUM_OBJECTS; i++) {
        world.addObject(object_t(
            1.0f + (((rand() % 100) / 100.0f) * 3.0f),                                           // radius
            sf::Vector2f(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT),                         // position
            sf::Vector2f((rand() % 100 - 50) / 100.0f, (rand() % 100 - 50) / 100.0f) * 1.5f,     // velocity
            sf::Color(rand() % 256, rand() % 256, rand() % 256)                                  // color
        ));
    }

    // Timing
    sf::Clock clock;
    sf::Clock frameClock;
    sf::Time lastUpdateTime;
    sf::Time lastRenderTime;
    sf::Time lastFrameTime;

    // Main game loop
    while (window.isOpen()) 
    {
        /////////////// Handle SFML events, close game etc..
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        /////////////// Update the world
        clock.restart();
        world.update();
        lastUpdateTime = clock.getElapsedTime();
        
        /////////////// Render the world
        clock.restart();
        window.clear();
        world.render(window);
        lastRenderTime = clock.getElapsedTime();

        /////////////// GUI
        window.draw(rect);

        std::stringstream ss;
        ss << std::fixed << std::setprecision(3);
        float total_time = (lastUpdateTime + lastRenderTime).asMicroseconds() / 1000.0f;
        float frame_time = frameClock.getElapsedTime().asMicroseconds() / 1000.0f;
        frameClock.restart();
        
        ss << "objects:   " << world.objects.size() << "\n"
           << "update:    " << lastUpdateTime.asMicroseconds() / 1000.0f << "ms\n"
           << "render:    " << lastRenderTime.asMicroseconds() / 1000.0f << "ms\n"
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
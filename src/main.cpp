#include <SFML/Graphics.hpp>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <iostream>

#include "world.hpp"

// Default values
constexpr uint32_t WINDOW_WIDTH  = 1800;
constexpr uint32_t WINDOW_HEIGHT = 1200;
constexpr uint32_t MAX_OBJECTS   = 14000;
constexpr uint32_t SPAWN_AMOUNT  = 0;
constexpr float    MIN_RADIUS    = 2.0f;
constexpr float    MAX_RADIUS    = 6.0f;
constexpr float    GRAVITY       = 200.0f;
constexpr bool     RAIN_MODE     = false;


int main(int argc, char *argv[])
{
    uint32_t width = WINDOW_WIDTH;
    uint32_t height = WINDOW_HEIGHT;
    uint32_t spawn_limit = MAX_OBJECTS;
    uint32_t spawn_amount = SPAWN_AMOUNT;
    float min_radius = MIN_RADIUS;
    float max_radius = MAX_RADIUS;
    float gravity = GRAVITY;
    bool  rain_mode = RAIN_MODE;

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--width") == 0 && (++i < argc)) {
            width = atoi(argv[i]);
        } else if (strcmp(argv[i], "--height") == 0 && (++i < argc)) {
            height = atoi(argv[i]);
        } else if (strcmp(argv[i], "--min-radius") == 0 && (++i < argc)) {
            min_radius = atof(argv[i]);
        } else if (strcmp(argv[i], "--max-radius") == 0 && (++i < argc)) {
            max_radius = atof(argv[i]);
        } else if (strcmp(argv[i], "--spawn-limit") == 0 && (++i < argc)) {
            spawn_limit = atoi(argv[i]);
        } else if (strcmp(argv[i], "--spawn-amount") == 0 && (++i< argc)) {
            spawn_amount = atoi(argv[i]);
        } else if (strcmp(argv[i], "--gravity") == 0 && (++i< argc)) {
            gravity = atof(argv[i]);
        } else if (strcmp(argv[i], "--rain") == 0) {
            rain_mode = true;
        } else if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "-h") == 0)) {
            std::cout << "Usage: ./big_physics_sim [options]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  --width <width>          Set the window width" << std::endl;
            std::cout << "  --height <height>        Set the window height" << std::endl;
            std::cout << "  --min-radius <radius>    Set the minimum circle radius" << std::endl;
            std::cout << "  --max-radius <radius>    Set the maximum circle radius" << std::endl;
            std::cout << "  --spawn-limit <amount>   Set the maximum number of objects" << std::endl;
            std::cout << "  --spawn-amount <amount>  Set the number of objects to spawn per frame (0 = spawn all objects at once)" << std::endl;
            std::cout << "  --gravity <value>        Set the gravity value" << std::endl;
            std::cout << "  --rain                   Enable rain mode" << std::endl;
            std::cout << "  --help, -h               Display this help message" << std::endl;
            return EXIT_SUCCESS;
        }
    }

    std::cout << "width: " << width << std::endl;
    std::cout << "height: " << height << std::endl;
    std::cout << "min_radius: " << min_radius << std::endl;
    std::cout << "max_radius: " << max_radius << std::endl;
    std::cout << "spawn_limit: " << spawn_limit << std::endl;
    std::cout << "spawn_amount: " << spawn_amount << std::endl;
    std::cout << "rain_mode: " << (rain_mode ? "true" : "false") << std::endl;
    std::cout << "gravity: " << gravity << std::endl;
    
    // Create a window
    sf::RenderWindow window(sf::VideoMode(width, height), "Big Physics Simulation");
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
    
    // Create world
    world_t world(width, height, gravity, min_radius, max_radius);

    // Spawn objects
    if (spawn_amount == 0) {
        for (uint32_t i = 0; i < spawn_limit; i++) {
            world.spawn_object();
        }
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

        if (world.objects.size() < spawn_limit) {
            for (uint32_t i = 0; i < spawn_amount; ++i) {   
                world.spawn_object(rand() % width, rain_mode ? 0 : rand() % height);
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
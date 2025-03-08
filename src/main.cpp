#include <SFML/Graphics.hpp>

int main() {
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Big Physics Simulation");

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        // Display everything that was drawn
        window.display();
    }

    return 0;
} 
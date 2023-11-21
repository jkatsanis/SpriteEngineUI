#include "Core.h"

void Core::hello() 
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    // Main loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // Close the window when the close button is clicked
                window.close();
            }
        }

        // Clear the window
        window.clear();

        // Draw something (e.g., a circle)
        sf::CircleShape circle(50.0f);
        circle.setFillColor(sf::Color::Green);
        circle.setPosition(375.0f, 275.0f); // Center the circle
        window.draw(circle);

        // Display the contents of the window
        window.display();
    }
    
}
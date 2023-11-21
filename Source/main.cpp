#include <SFML/Graphics.hpp>

int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    // Main loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // Close the window if the close button is clicked
                window.close();
            }
        }

        // Clear the window
        window.clear();

        // Draw your content here

        // Display the contents of the window
        window.display();
    }

    return 0;
}

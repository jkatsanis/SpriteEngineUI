#include <SFML/Graphics.hpp>
#include <light/lightRepository.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Light Example");

    // Load textures
    sf::Texture lightTexture;
    lightTexture.loadFromFile("player.png");

    sf::Texture d;
    d.loadFromFile("Unbenannt.PNG");
    sf::Sprite ds;
    ds.setTexture(d);

    s2d::LightRepository repo;
    repo.add(s2d::Vector2(50, 50), 100, 1, s2d::Vector3(0, 0.5f, 1), "First");


    s2d::Vector2 position;

    float playerSpeed = 0.5f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            position.y += playerSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            position.x -= playerSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            position.y -= playerSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            position.x += playerSpeed;

        repo.moveLightSource("First", position);

        window.clear(sf::Color(0, 0, 0));

        window.draw(ds, &repo.getShader());

        window.display();
    }

    return 0;
}
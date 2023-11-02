#include <SFML/Graphics.hpp>
#include <manager/lightRepository.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML Light Example");

    // Load textures
    sf::Texture lightTexture;
    lightTexture.loadFromFile("player.png");

    sf::Texture d;
    d.loadFromFile("Unbenannt.PNG");
    sf::Sprite ds;
    ds.setTexture(d);

    s2d::Vector2 position = s2d::Vector2(0, 0);


    s2d::LightRepository::init();
    s2d::LightRepository::add(position, 500, 10, s2d::Vector3(0, 0.5f, 1), "First");

    sf::Vector2f cam_pos = sf::Vector2f(960, 540);

    sf::View view = sf::View(cam_pos, sf::Vector2f(1920, 1080));

    float playerSpeed = 0.5f;

    window.setView(view);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            position.y -= playerSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            position.x -= playerSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            position.y += playerSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            position.x += playerSpeed;

        float cameraSpeed = 1.0f;
        // Camera movement controls using arrow keys
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            cam_pos.x -= cameraSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            cam_pos.x += cameraSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            cam_pos.y -= cameraSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            cam_pos.y += cameraSpeed;
        }

        // Update the center of the view with the new position
        view.setCenter(cam_pos);

        sf::Shader& shader = s2d::LightRepository::getShader();

        s2d::LightRepository::moveLightSource("First", position);

        shader.setUniform("cameraPosition", sf::Vector2f(cam_pos.x - 960, cam_pos.y - 540));

        window.clear(sf::Color(0, 0, 0));

        window.draw(ds, &shader);
        window.setView(view);
        window.display();
    }

    return 0;
}
#include <SFML/Graphics.hpp>

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

    // Create light source sprites
    sf::Sprite lightSource;
    lightSource.setTexture(lightTexture);
    lightSource.setPosition(300, 300);

    sf::Sprite lightSource2;
    lightSource2.setTexture(lightTexture);
    lightSource2.setPosition(0, 50);







    const int maxLights = 2; // You can change this to support more lights if needed
    sf::Vector2f lightPositions[maxLights];
    float lightRadii[maxLights];
    float lightIntensities[maxLights];
    sf::Vector3f lightColors[maxLights];

    // Initialize light properties for two lights (you can add more if needed)
    lightPositions[0] = sf::Vector2f(100.0f, 100.0f);
    lightRadii[0] = 200.0f;
    lightIntensities[0] = 1.0f;
    lightColors[0] = sf::Vector3f(1.0f, 0.0f, 0.0f); // Red light

    lightPositions[1] = sf::Vector2f(300.0f, 300.0f);
    lightRadii[1] = 150.0f;
    lightIntensities[1] = 0.7f;
    lightColors[1] = sf::Vector3f(0.0f, 1.0f, 0.0f); // Green light


    // Create the light shader
    sf::Shader lightShader;
    lightShader.loadFromFile("ressources/shaders/circulaer_gradient.frag", sf::Shader::Fragment);


    lightShader.setUniformArray("lightPositions", lightPositions, maxLights);
    lightShader.setUniformArray("lightRadii", lightRadii, maxLights);
    lightShader.setUniformArray("lightIntensities", lightIntensities, maxLights);
    lightShader.setUniformArray("lightColors", lightColors, maxLights);

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
            lightSource.move(0, -playerSpeed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            lightSource.move(-playerSpeed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            lightSource.move(0, playerSpeed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            lightSource.move(playerSpeed, 0);


        window.clear(sf::Color(0, 0, 0));

        window.draw(ds, &lightShader);

        window.draw(lightSource2, &lightShader);

        window.draw(lightSource, &lightShader);

 
        window.display();
    }

    return 0;
}

#include <SpriteEngine.h>   

int main()
{
    s2d::GameEngine engine;

    while (engine.ptr_renderWindow->isOpen())
    { 
        if (s2d::Input::onKeyPress(s2d::KeyBoardCode::A))
        {
            std::cout << s2d::Sprite::activeSprites[0]->getSprite().getPosition().x << std::endl;
        }
        engine.update();
    }

    ImGui::SFML::Shutdown();
    return 0;
}


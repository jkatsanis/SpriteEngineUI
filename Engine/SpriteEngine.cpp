#include <SpriteEngine.h>   

int main()
{
    s2d::GameEngine engine;

    while (engine.ptr_renderWindow->isOpen())
    { 
        if (s2d::Input::onKeyPress(s2d::KeyBoardCode::A))
        {
            for (s2d::Animation& anim : s2d::Sprite::activeSprites[0]->animator.animations)
            {
                std::cout << anim.name;
            }
        }
        engine.update();
    }

    ImGui::SFML::Shutdown();
    return 0;
}


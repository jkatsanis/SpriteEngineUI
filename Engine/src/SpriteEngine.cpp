#include <SpriteEngine.h>   
#include <data/engineData.h>

int main()
{
    s2d::UIProjectSelector selector;

    while (selector.isOpen())
    {
        selector.update();  
    }

    // search where the error happens when u selet a invalid project
    if (s2d::EngineData::s_pathToUserProject == "")
    {
        // Closing the engine since the user doesnt want to open a project;
        return -1;
    }
    int x = 1;
    s2d::GameEngine engine;

    while (engine.ptr_renderWindow->isOpen())
    { 
        if (s2d::Input::onKeyPress(s2d::KeyBoardCode::A))
        {
            s2d::Sprite* spr = s2d::Sprite::activeSprites[3];

            spr->getSprite().setOrigin(100, 71);
            spr->getSprite().setScale(1, 1);
            spr->getSprite().setOrigin(0, 0);

            sf::IntRect textureRect = spr->getSprite().getTextureRect();
            spr->transform.textureSize = s2d::Vector2(textureRect.width * 1, textureRect.height * 1);
        }
        if (s2d::Input::onKeyPress(s2d::KeyBoardCode::B))
        {
            s2d::Vector2 scale = { -2, 1 };
            s2d::Sprite* spr = s2d::Sprite::activeSprites[3];
            spr->getSprite().setOrigin(100, 71);
            spr->getSprite().setScale(scale.x, scale.y);
            spr->getSprite().setOrigin(200, 0);

            if (scale.x < 0)
            {
                scale.x *= -1;
            }

            sf::IntRect textureRect = spr->getSprite().getTextureRect();
            spr->transform.textureSize = s2d::Vector2(textureRect.width * scale.x, textureRect.height * scale.y);
        }
        engine.update();
    }

    return 0;
}


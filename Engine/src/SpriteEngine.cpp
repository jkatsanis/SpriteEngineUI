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
    s2d::GameEngine engine;

    s2d::Sprite* spr = s2d::Sprite::activeSprites[0];

    std::string x = s2d::EngineData::s_pathToUserProject + "\\assets\\Sprites\\big.png";
    std::string y = s2d::EngineData::s_pathToUserProject + "\\assets\\Sprites\\down.png";

    std::vector<std::string> path = { x, y };

    spr->animator.createAnimation("name", path, 150, true);

    while (engine.ptr_renderWindow->isOpen())
    {  
        if (s2d::Input::onKeyPress(s2d::KeyBoardCode::A))
        {
            spr->animator.play("name");
        }
    
        engine.update();
    }

    return 0;
}


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
            s2d::Vector2 scale = s2d::Vector2(x + 2, x + 2);
            x++;
            s2d::Sprite::activeSprites[2]->transform.setScale(scale);
        }
        engine.update();
    }

    return 0;
}


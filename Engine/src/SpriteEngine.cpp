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
    s2d::EngineData::s_pathToUserProject = "..\\Assets";

    s2d::GameEngine engine;

    while (engine.ptr_renderWindow->isOpen())
    { 
        std::cout << s2d::Sprite::activeSprites[0]->path << std::endl;
        engine.update();
    }

    return 0;
}


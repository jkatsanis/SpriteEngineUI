#include <SpriteEngine.h>   
#include <data/engineData.h>

int main()
{
    s2d::EngineData::s_pathToUserProject = "..\\Assets";

    s2d::GameEngine engine;

    while (engine.ptr_renderWindow->isOpen())
    { 
        engine.update();
    }

    return 0;
}


#include <SpriteEngine.h>   
#include <data/engineData.h>

int main()
{
    s2d::UIProjectSelector selector;

    while (selector.isOpen())
    {
        selector.update();  
    }

    if (s2d::EngineData::s_pathToUserProject == "")
    {
        // Closing the engine since the user doesnt want to open a project;
        return -1;
    }

    s2d::GameEngine engine;

    if (engine.ptr_renderWindow == nullptr)
    {
        std::cout << "Window was nullptr, please rebuild!!" << std::endl;
        exit(-1);
    }
    while (engine.ptr_renderWindow->isOpen())
    {    
        engine.update();
    }

    return 0;
}


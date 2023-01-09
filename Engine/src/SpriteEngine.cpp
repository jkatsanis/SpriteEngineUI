#include <SpriteEngine.h>   

int main()
{
    s2d::UIProjectSelector projectSelector;

    while (projectSelector.isOpen())
    {
        projectSelector.update();
    }

    if (projectSelector.project.name == "none")
    {
        return -1;
    }

    s2d::GameEngine engine;

    while (engine.ptr_renderWindow->isOpen())
    { 
        engine.update();
    }

    return 0;
}


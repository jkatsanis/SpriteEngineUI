#include <SpriteEngine.h>   
#include <direct.h>

int main()
{
    //s2d::UIProjectSelector projectSelector;

    //while (projectSelector.isOpen())
    //{
    //    projectSelector.update();
    //}

    //if (projectSelector.project.name == "none")
    //{
    //    return -1;
    //}


    s2d::ProjectInfo::s_pathToUserProject = "..\\Assets";

    s2d::GameEngine engine;

    while (engine.ptr_renderWindow->isOpen())
    { 
        engine.update();
    }

    return 0;
}


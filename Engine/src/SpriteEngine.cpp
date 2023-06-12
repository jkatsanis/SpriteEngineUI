#include <SpriteEngine.h>   

int main()
{
    s2d::UIProjectSelector selector;

    while (selector.isOpen())
    {
        selector.update();
    }

    if (s2d::EngineData::s_path_to_user_project == "")
    {
        // Closing the engine since the user doesnt want to open a project;
        return -1;
    }

    s2d::GameEngine engine;

    if (engine.ptr_render_window == nullptr)
    {
        std::cout << "Window was nullptr, please rebuild!!" << std::endl;
        return -1;
    }
    while (engine.ptr_render_window->isOpen())
    {
        engine.update();
    }
    return 0;
}


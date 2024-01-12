#include "Editor.h"
#include "GUI/ProjectSelector/UIProjectSelector.h"

int main()
{
    // Sets the engine data to load the game 
    spe::UIProjectSelector selector;

    while (selector.IsOpen())
    {
        selector.Update();
    }

    selector.Shutdown();

    if (spe::EngineData::s_PathUserProject == "")
    {
        spe::Log::LogString("Closing engine, no project selected");
        return 0;
    }

    spe::Editor editr;

    while (editr.IsOpen())
    {
        editr.Update();
    }

    return 0;
}
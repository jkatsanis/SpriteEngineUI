#include "Editor.h"
#include "GUI/ProjectSelector/UIProjectSelector.h"

#ifdef __linux__
#include <X11/Xlib.h>

int ignoreXErrors(Display* d, XErrorEvent* e) {
    std::cerr << "X11 Error " << e->error_code << " ignored\n";
    return 0;
}

#endif

int main()
{
#ifdef __linux__
    XSetErrorHandler(ignoreXErrors);
#endif

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
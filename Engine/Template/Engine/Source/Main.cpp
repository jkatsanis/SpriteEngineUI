#include <iostream>

#include "Engine.h"

int main() 
{
    spe::Engine engine;
    while (engine.IsOpen())
    {
        engine.Update();
    }

    return 0;
}

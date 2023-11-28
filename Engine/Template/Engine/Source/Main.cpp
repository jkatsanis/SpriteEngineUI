#include <iostream>

#include "Engine.h"

#include <Include/SFML/Graphics.hpp>

int main() 
{
    spe::Engine engine;
    while (engine.IsOpen())
    {
        engine.Update();
    }

    return 0;
}

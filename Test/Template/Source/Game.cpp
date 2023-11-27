#include <iostream>

#include "Source/Core/GameWindow.h"

#include <Include/SFML/Graphics.hpp>

int main() 
{
    spe::GameWindow window(spe::Vector2(1920, 1080), "s");
    while (window.IsOpen())
    {
        window.PollEvents();
        window.Clear();
        window.Display();
    }
    return 0;
}

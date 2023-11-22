#include "Editor.h"

int main() 
{
    spe::Editor editor;

    while (editor.IsOpen())
    {
        editor.Update();
    }

    return 0;
}


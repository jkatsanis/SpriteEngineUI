#include "Editor.h"

spe::Editor::Editor()
{
	this->m_Window = spe::Window(spe::Vector2(1920, 1080), "SpriteEngine");
}

void spe::Editor::Update()
{
	this->m_Window.Draw();
}

#include "UIWindow.h"

// Public

void spe::UIWindow::Update()
{
	this->m_UIHierarchy.Render();
}

void spe::UIWindow::SetRepos(spe::GUIRepository& gui, spe::SpriteRepository& sprite)
{
	this->m_UIHierarchy.SetRepos(&sprite, &gui);
}

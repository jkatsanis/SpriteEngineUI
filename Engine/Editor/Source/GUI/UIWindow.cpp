#include "UIWindow.h"

// Public

void spe::UIWindow::Update()
{
	this->m_UIHierarchy.Render();
	this->m_UITopbar.Render();
	this->m_UIInspector.Render();
}

void spe::UIWindow::SetRepos(spe::GUIRepository& gui, spe::SpriteRepository& sprite, spe::SceneHandler& scene)
{
	this->m_UIHierarchy.SetRepos(&sprite, &gui);
	this->m_UITopbar.SetRepos(&sprite, &gui);
	this->m_UIInspector.SetRepos(&sprite, &gui);

	this->m_UITopbar.SetSceneHandler(scene);
}

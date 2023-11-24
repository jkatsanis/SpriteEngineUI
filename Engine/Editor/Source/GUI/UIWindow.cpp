#include "UIWindow.h"

// Public

void spe::UIWindow::Update()
{
	this->m_UIHierarchy.Render();
	this->m_UITopbar.Render();
	this->m_UIInspector.Render();
	this->m_UIAnimation.Render();


	if (this->m_UIHierarchy.Hovered || this->m_UITopbar.Hovered || this->m_UIInspector.Hovered 
		|| this->m_UIAnimation.Hovered)
	{
		spe::UIUtility::s_IsAnyHovered = true;
	}
	else 
	{
		spe::UIUtility::s_IsAnyHovered = false;
	}
}

void spe::UIWindow::SetRepos(spe::GUIRepository& gui, spe::SpriteRepository& sprite, spe::SceneHandler& scene)
{
	this->m_UIHierarchy.SetRepos(&sprite, &gui);
	this->m_UITopbar.SetRepos(&sprite, &gui);
	this->m_UIInspector.SetRepos(&sprite, &gui);
	this->m_UIAnimation.SetRepos(&sprite, &gui);

	this->m_UITopbar.SetSceneHandler(scene);
}

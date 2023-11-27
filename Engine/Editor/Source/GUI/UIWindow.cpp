#include "UIWindow.h"

// Public

void spe::UIWindow::Update()
{
	this->m_UIHierarchy.Render();
	this->m_UITopbar.Render();
	this->m_UIInspector.Render();
	this->m_UIAnimation.Render();
	this->m_UIAssetFolder.Render();


	if (this->m_UIHierarchy.Hovered || this->m_UITopbar.Hovered || this->m_UIInspector.Hovered 
		|| this->m_UIAnimation.Hovered || this->m_UIAssetFolder.Hovered)
	{
		spe::UIUtility::s_IsAnyHovered = true;
	}
	else 
	{
		spe::UIUtility::s_IsAnyHovered = false;
	}
}

void spe::UIWindow::SetRepos(spe::GUIRepository& gui, spe::SpriteRepository& sprite, spe::SceneHandler& scene, spe::LightRepository& light)
{
	this->m_UIHierarchy.SetRepos(&sprite, &gui);

	this->m_UIHierarchy.SetLightRepository(light);

	this->m_UITopbar.SetRepos(&sprite, &gui);
	this->m_UIInspector.SetRepos(&sprite, &gui);
	this->m_UIAnimation.SetRepos(&sprite, &gui);
	this->m_UIAssetFolder.SetRepos(&sprite, &gui);

	this->m_UITopbar.SetSceneHandler(scene);
}

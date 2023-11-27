#pragma once

#include "UITopbar.h"

#include "SpriteRepository.h"
#include "GUIRepository.h"
#include "Property/UIHierarchy.h"
#include "Core/SeceneHandler.h"
#include "Property/Inspector/UIInspector.h"
#include "Property/Animations/UIAnimation.h"
#include "Property/AssetFolder/UIAssetFolder.h"

namespace spe
{
	class UIWindow
	{
	private:
		spe::UITopbar m_UITopbar;
		spe::UIHierarchy m_UIHierarchy;
		spe::UIInspector m_UIInspector;
		spe::UIAnimation m_UIAnimation;
		spe::UIAssetFolder m_UIAssetFolder;

	public:

		void Update();

		void SetRepos(spe::GUIRepository& gui, spe::SpriteRepository& sprite, spe::SceneHandler& scene, spe::LightRepository& light);
	};
}
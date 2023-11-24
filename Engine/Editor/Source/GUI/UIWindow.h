#pragma once

#include "UITopbar.h"

#include "SpriteRepository.h"
#include "GUIRepository.h"
#include "Property/UIHierarchy.h"
#include "Core/SeceneHandler.h"

namespace spe
{
	class UIWindow
	{
	private:
		spe::UITopbar m_UITopbar;
		spe::UIHierarchy m_UIHierarchy;

	public:

		void Update();

		void SetRepos(spe::GUIRepository& gui, spe::SpriteRepository& sprite, spe::SceneHandler& scene);
	};
}
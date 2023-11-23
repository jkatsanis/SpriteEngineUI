#pragma once

#include "UITopbar.h"

#include "SpriteRepository.h"
#include "GUIRepository.h"
#include "Property/UIHierarchy.h"

namespace spe
{
	class UIWindow
	{
	private:
		spe::UITopbar m_Topbar;
		spe::UIHierarchy m_UIHierarchy;

	public:

		void Update();

		void SetRepos(spe::GUIRepository& gui, spe::SpriteRepository& sprite);
	};
}
#pragma once

#include "UITopbar.h"

#include "SpriteRepository.h"
#include "GUIRepository.h"

namespace spe
{
	class UIWindow
	{
	private:
		spe::UITopbar m_Topbar;

	public:

		void Update();

		void SetRepos(spe::GUIRepository& gui, spe::SpriteRepository& sprite);
	};
}
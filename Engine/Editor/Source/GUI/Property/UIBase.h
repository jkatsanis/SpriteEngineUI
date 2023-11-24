#pragma once

#include "Math/Vector2.h"
#include "Sprite/SpriteRepository.h"
#include "GUI/GUIRepository.h"

namespace spe
{
	class IUIBase
	{
	protected:
		ImVec2 m_Size;
		spe::SpriteRepository* m_ptr_Repo;
		spe::GUIRepository* m_ptr_GUIRepo;

		virtual void Init() = 0;
		IUIBase() : Hovered(false), m_ptr_Repo(nullptr), m_ptr_GUIRepo(nullptr) { }
	public:
		bool Hovered;

		virtual void Render() = 0;

		void SetRepos(spe::SpriteRepository* sprites, spe::GUIRepository* gui)
		{
			this->m_ptr_Repo = sprites;
			this->m_ptr_GUIRepo = gui;
			this->Init();
		}

	};
}
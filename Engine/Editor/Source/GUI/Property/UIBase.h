#pragma once

#include "Math/Vector2.h"
#include "Sprite/SpriteRepository.h"
#include "GUI/GUIRepository.h"

namespace spe
{
	class IUIBase
	{
	protected:
		spe::Vector2 m_Size;
		spe::SpriteRepository* m_ptr_Repo;
		spe::GUIRepository* m_ptr_GUIRepo;

		virtual void Init() = 0;
		IUIBase() : m_ptr_Repo(nullptr), m_ptr_GUIRepo(nullptr) { }
	public:

		virtual void Render() = 0;

		void SetRepos(spe::SpriteRepository* sprites, spe::GUIRepository* gui)
		{
			this->m_ptr_Repo = sprites;
			this->m_ptr_GUIRepo = gui;
		}

	};
}
#pragma once

#include <manager/spriteRepository.h>
#include <guiComponents/UI.h>

#include <ImGui.h>

namespace s2d
{
	class UITagSelector
	{
	private:
		s2d::SpriteRepository* m_ptr_sprite_repo;
		bool m_open_name_popup;
		char m_tag_name[150];

		void renderSelector();
		void renderPopup(bool* hovered);

	public:
		UITagSelector();

		void update(bool* hovered);

		void setSpriteRepo(s2d::SpriteRepository* repo) { this->m_ptr_sprite_repo = repo; }
	};
}


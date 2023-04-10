#pragma once

#include <iostream>
#include <ImGui.h>

#include <physicalComponents/sprite.h>

#define WINDOW_SIZE ImVec2(250.0f, 1080.0f)
#define WINDOW_POS ImVec2(0.0f, 0.0f)

#define POPUP_NAME "CONTEXT_MENU"

namespace s2d
{
	class UIHierarchy
	{
	private:
		s2d::SpriteRepository* m_ptr_repo;

	public:		
		bool isHovered;

		UIHierarchy();
		UIHierarchy(s2d::SpriteRepository& repo);

		void displayHierarchyWindow();

		void displayContextPopup();
		void displaySprites();
		void displaySprites(s2d::Sprite* parent, bool& anyHovered);

		void addSprite();
		void deleteSprite();
	};
}


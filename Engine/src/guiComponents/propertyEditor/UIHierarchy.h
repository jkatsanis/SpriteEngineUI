#pragma once

#include <iostream>
#include <ImGui.h>

#include <physicalComponents/sprite.h>

#define WINDOW_SIZE ImVec2(250.0f, 1080.0f)
#define WINDOW_POS ImVec2(0.0f, 0.0f)
#define SPRITE_SELECTED_COLOR ImVec4(139.0f / 255.0f, 180.0f / 255.0f, 234.0f / 255.0f,1.0f)

#define POPUP_NAME "CONTEXT_MENU"

namespace s2d
{
	class UIHierarchy
	{
	private:
		s2d::SpriteRepository* m_ptr_repo;
		bool m_waitOneFrame;

		void displayContextPopup();
		bool displaySprites();
		void displaySprites(s2d::Sprite* parent, bool& anyHovered);
		void displayChildToParent();
		void displaySpriteSeperated(s2d::Sprite* d, bool& anyHovered);

		void setSpriteAsChild();

		void addSprite();
		void deleteSprite();

		void cleanRepoSpritesUp(bool isAnyHovered);

		void setMenuitemHovered(bool& any_hovered, s2d::Sprite* sprite);

	public:		
		bool isHovered;

		UIHierarchy();
		UIHierarchy(s2d::SpriteRepository& repo);
		void displayHierarchyWindow();
	};
}


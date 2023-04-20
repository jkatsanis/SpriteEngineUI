#pragma once

#include <iostream>
#include <ImGui.h>

#include <physicalComponents/sprite.h>
#include <data/UIInfo.h>
#include <UIToolButtons.h>

#define WINDOW_POS ImVec2(0.0f, WINDOW_SIZE_Y_TOOL_BUTTONS + 38)
#define SPRITE_SELECTED_COLOR ImVec4(139.0f / 255.0f, 180.0f / 255.0f, 234.0f / 255.0f,1.0f)

#define POPUP_NAME "CONTEXT_MENU"
#define WINDOW_SIZE_HIERARCHY_Y 1080 - WINDOW_POS.y
#define FOLDER_SPRITE_HIERARCHY_PADDING 50

#define MENU_ITEM_PADDING 50
#define TREE_NODE_PADDING 40

#define ADD_WHEN_SPRITE_HAS_PARENT 30

// x amount of s to select a child to parent
#define TIME_TO_CAN_SELECT_CHILD 0.3f

namespace s2d
{
	class UIHierarchy
	{
	private:
		s2d::SpriteRepository* m_ptr_repo;
		bool m_waitOneFrame;
		ImVec2 m_windowSize;
		ImGuiTextFilter m_searchSpriteFilter;
		uint8_t m_spriteBackgRoundRowCounter;

		/// <summary>
		/// Not a macro because it can get changed!!
		/// </summary>
		float m_windowSizeX;

		const ImVec2* m_ptr_assetWindowSize;
		bool m_foundHovering;
		bool m_clickedOnResizeButton;
		float m_childSelectTimer;

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

		void addPrefab();
		void renderCloseRectangle();
		void renderHierarchyOptions();
		void resizeWindow();

		void drawbackgroundRectangle();


		/// <summary>
		/// Checks for specfic things which get hovered 
		/// </summary>
		void setHovering(s2d::Sprite* sprite, bool& anyHovered);

	public:		
		bool isHovered;

		UIHierarchy();
		UIHierarchy(s2d::SpriteRepository& repo);
		void displayHierarchyWindow();

		void setPtrToWindow(const ImVec2* ptr) { this->m_ptr_assetWindowSize = ptr; }
	};
}


#pragma once

#include <iostream>
#include <ImGui.h>

#include "UIBase.h"
#include "Sprite/Sprite.h"
#include "GUI/GUIRepository.h"
#include "GUI/UIUtility/UIUtility.h"
#include "Utility/Style.h"
#include "GUI/UIUtility/UIModels.h"
#include "Include/icons.h"
#include "RessourceHandler/Initializer.h"

#define SPRITE_SELECTED_COLOR ImVec4(139.0f / 255.0f, 180.0f / 255.0f, 234.0f / 255.0f,1.0f)

#define POPUP_NAME "CONTEXT_MENU"
#define WINDOW_SIZE_HIERARCHY_Y 1080 - WINDOW_POS.y
#define FOLDER_SPRITE_HIERARCHY_PADDING 50

#define MENU_ITEM_PADDING 50
#define TREE_NODE_PADDING 40

#define ADD_WHEN_SPRITE_HAS_PARENT 30

#define SPRITE_BACKGROUND_COLOR ImColor(30, 30, 30, 255)

#define WINDOW_POS ImVec2(0.0f, WINDOW_SIZE_Y_TOOL_BUTTONS + 38)
#define HIERARCHY_DEFAULT_WINDOW_SIZE ImVec2(300.0f, 1080.0f - 350.0f)
// x amount of s to select a child to parent
#define TIME_TO_CAN_SELECT_CHILD 0.3f

namespace spe
{
	class UIHierarchy : public spe::IUIBase
	{
	private:
		spe::ResizeWindowData m_ResizeData;
		bool m_WaitFrame;
		ImGuiTextFilter m_SearchFilter;

		bool m_FoundHovering;
		bool m_FoundSelected;
		float m_ChildSelectTimer;
		uint8_t m_BackgroundColorCnt;

		spe::LightRepository* m_ptr_LightRepo;

		void DisplayContextPopup();
		bool DisplaySprites();
		void DisplaySprites(spe::Sprite* parent, bool& any_hovered);
		void DisplayChildToParent();
		void DisplaySpriteSeperated(spe::Sprite* d, bool& any_hovered);

		void SetSelectedBackgroundColor(spe::Sprite* sprite, bool& pop_style);
		void SetSpriteAsChild();

		void AddSprite();
		void DeleteSprite();
		void Unparent();
		void CleanRepoSpritesUp(bool any_hovered);
		void SetMenuitemHovered(bool& any_hovered, spe::Sprite* sprite);

		void AddPrefab();
		void RenderCloseRectangle();
		void RenderHierarchyOptions();
		void ResizeWindow();

		void DrawbackgroundRectangle();
		void SetSpriteOnClick(spe::Sprite* sprite);
		void DrawUIRactangleWhenHovered(spe::Sprite* sprite);

		void DrawRenderSymbol(spe::Sprite* child);

		void OnSpriteAdd(spe::Sprite* spr);

		/// <summary>
		/// Checks for specfic things which get hovered 
		/// </summary>
		void SetHovering(spe::Sprite* sprite, bool& any_hovered);

		void CopySprite();

		/// <summary>
		/// Gets called from base class
		/// </summary>
		void Init() override;
	public:
		UIHierarchy();

		void Render() override;

		void SetLightRepository(spe::LightRepository& light) { this->m_ptr_LightRepo = &light; }
	};
}


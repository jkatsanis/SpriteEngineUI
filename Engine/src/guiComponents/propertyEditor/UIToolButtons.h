#pragma once

#include <engineComponents/input.h>
#include <ImGui.h>
#include <icons.h>
#include <SFML/Graphics.hpp>
#include <manager/saveSystem.h>
#include <data/UIInfo.h>
#include <manager/fontManager.h>
#include <data/engineData.h>
#include <Windows.h>
#include <guiComponents/colors.h>
#include <UIToolButtonsModels.h>
#include <filesystem>
#include <manager/saveSystem.h>
#include <manager/spriteRepository.h>

#define TOOLS_SIZE 2
#define FILE_AMOUNT 6

#define WINDOW_SIZE_Y_TOOL_BUTTONS 15.0f

namespace s2d
{
	class UIToolButtons
	{
	private:
		s2d::Tool m_tools[TOOLS_SIZE];
		s2d::Vector3 m_window_background_to_save;
		EditorTools m_editor_tools;
		s2d::SpriteRepository* m_ptr_repo;

		bool m_clicked_on_btn;

		void askWithButtonForPlayGame();
		void toolSelector();
		void removeBackgroundFromButtons();
		void buildProjectIntoFolder();
		void hotkeys();
		void build();
		void init();
		void renderWindowSelecter();
		void renderMainMenuBar();
	public:
		bool is_hovered;

		UIToolButtons();
		UIToolButtons(s2d::SpriteRepository& sprite_repo);

		void createToolsAndButtons();
		void setBackgroundColorToSave(const s2d::Vector3& color);
	};
}


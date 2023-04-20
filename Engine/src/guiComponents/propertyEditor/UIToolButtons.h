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
		s2d::Vector3 m_windowbBackgroundToSave;
		EditorTools m_editor_tools;
		s2d::SpriteRepository* m_spriteRepository;

		bool m_clickedOnBtn;

		void askWithButtonForPlayGame();
		void toolSelector();
		void removeBackgroundFromButtons();
		void buildProjectIntoFolder();
		void hotkeys();
		void build();
		void init();

		void renderMainMenuBar();
	public:
		bool isHovered;

		UIToolButtons();
		UIToolButtons(s2d::SpriteRepository& spriteRepo);

		void createToolsAndButtons();
		void setBackgroundColorToSave(const s2d::Vector3& color);
	};
}


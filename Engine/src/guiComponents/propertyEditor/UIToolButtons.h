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

#define TOOLS_SIZE 2

namespace s2d
{
	class UIToolButtons
	{
	private:
		s2d::Tool m_tools[TOOLS_SIZE];
		s2d::Vector3 m_windowbBackgroundToSave;

		bool m_clickedOnBtn;

		void askWithButtonForPlayGame();
		void toolSelector();
		void removeBackgroundFromButtons();

	public:
		bool isHovered;
		EditorTools editorTools;

		UIToolButtons();

		void createToolsAndButtons();

		void setBackgroundColorToSave(const s2d::Vector3& color);
	};
}


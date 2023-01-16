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

//Inlcluding game.h here, because we need to be able to create from here the game since here is the start button

namespace s2d
{
	class UIToolButtons
	{
	private:
		s2d::Vector3 m_windowbBackgroundToSave;

		void askWithButtonForPlayGame();
	public:
		bool isHovered;

		UIToolButtons();

		void createToolsAndButtons();

		void setBackgroundColorToSave(s2d::Vector3 color);
	};
}


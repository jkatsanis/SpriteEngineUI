#pragma once

#include <iostream>
#include <ImGui.h>
#include <sprite.h>
#include <string>
#include <icons.h>
#include <input.h>
#include <UIInfo.h>
#include <fontManager.h>
#include <UIHirachySpriteDisplay.h>
#include <UIHirachyMenuDisplayer.h>

namespace s2d
{
	class UIHirachy
	{
	private:

		s2d::UIHirachyMenuDisplayer m_menuDisplayer;
		s2d::UIHirachySpriteDisplayer m_spriteDisplayer;

		/// <summary>
		/// This method display the Hirachy components
		/// 1. All the sprites and their click functions
		/// 2. The pop up and delete functions
		/// -> gets called in 'createHirachyWindow'
		/// </summary>
		void displayHirachyComponents();

	public:		
		bool isHovered;

		UIHirachy();

		void createHirachyWindow();

	public:
		static s2d::Sprite* selectedSprite;
	};
}


#pragma once

#include <iostream>
#include <ImGui.h>
#include <sprite.h>
#include <string>
#include <icons.h>
#include <input.h>
#include <UIInfo.h>
#include <fontManager.h>

namespace s2d
{
	class UIHirachy
	{
	private:
		s2d::Sprite* m_tempChildSprite;
		s2d::Sprite* m_deleteSprite;

		const char* m_menuName;
		bool m_isPopUpOpen;
		bool m_isAnySpriteRightClicked;
		float m_width;
		bool m_clickedValidSprite;

		void render();
		void createMenuPopup();
		void renderMenuPopup();
		void addSpritesToHirachy();

		void displayChildsRecursivly(s2d::Sprite* sprite);
		void childSystem(s2d::Sprite* sprite, bool isHEader);

		void deleteChildsRecursivly(s2d::Sprite* toDelete);

		void createButton();
		void deleteButton();

	public:		
		bool isHovered;

		UIHirachy();

		void createHirachyWindow();

	public:
		static s2d::Sprite* selectedSprite;

	};
}


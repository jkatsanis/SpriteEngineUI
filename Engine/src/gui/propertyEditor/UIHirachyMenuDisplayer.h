#pragma once

#include <sprite.h>
#include <ImGui.h>

namespace s2d
{
	class UIHirachySpriteDisplayer;
	class UIHirachyMenuDisplayer
	{
	private:
		const char* m_menuName;
	  UIHirachySpriteDisplayer* m_spriteDisplayer;

		/// <summary>
		/// This method checks if a 'ImGui::Popup' is open. It will then call the 'createButton' and 'deleteButton' method.
		/// -> Gets called in 'createMenuPopup' 
		/// </summary>
		void renderMenuPopup();


		/// <summary>
		/// Deletes the sprite childs recursivly and changes its 'vecpos'
		/// -> gets called in 'deleteButton'
		/// </summary>
		///<param name="toDelete"> The parent which needs to be fully deletet </param>
		void deleteChildsRecursivly(s2d::Sprite* toDelete);

		void createButton();
		void deleteButton();

	public:
		bool isPopUpOpen;

		UIHirachyMenuDisplayer();
		UIHirachyMenuDisplayer(UIHirachySpriteDisplayer* spriteDisplayer);

		/// <summary>
		/// This method checks any mouse inputs which can either close or open a popup. It will also call the 'renderMenuPopup'
		/// method to render it if its currently open
		/// -> gets called in 'displayHirachyComponents'
		/// </summary>
		void createMenuPopup();
	};
}
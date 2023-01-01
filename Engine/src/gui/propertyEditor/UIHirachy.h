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
		// The sprite we clicked to drag into a parent
		s2d::Sprite* m_deleteSprite;

		const char* m_menuName;
		bool m_isPopUpOpen;
		bool m_isAnySpriteRightClicked;
		float m_width;
		bool m_clickedValidSprite;
		s2d::Sprite* m_childSelectedToParent;

		ImVec4 m_spriteSelectedColor;
		bool m_setAllKeepOnHirachyBoolsToFalse;



		/// <summary>
		/// This method display the Hirachy components
		/// 1. All the sprites and their click functions
		/// 2. The pop up and delete functions
		/// -> gets called in 'createHirachyWindow'
		/// </summary>
		void displayHirachyComponents();

		/// <summary>
		/// This method checks any mouse inputs which can either close or open a popup. It will also call the 'renderMenuPopup'
		/// method to render it if its currently open
		/// -> gets called in 'displayHirachyComponents'
		/// </summary>
		void createMenuPopup();


		/// <summary>
		/// This method checks if a 'ImGui::Popup' is open. It will then call the 'createButton' and 'deleteButton' method.
		/// -> Gets called in 'createMenuPopup' 
		/// </summary>
		void renderMenuPopup();

		/// <summary>
		/// This method iterates over all sprites WHICH ARE PARENTS and it will call the 'displayChildsRecursivly' function. It second job is to 
		/// check if we clicked at no valid item, thenit should set the 'selectedSprite' to nullptr. This will display a default inspector view.
		/// -> Gets called in 'displayHirachyComponents' 
		/// </summary>
		void addSpritesToHirachy();

		/// <summary>
		/// This method iterated over all childs of a parent recursivly. It's main job its to display the parent as TreeNodes and the Child as
		/// Menu Items. 
		/// -> Gets called in 'addSpritesToHirachy'
		/// </summary>
		/// <param name="sprite"></param>
		void displayChildsRecursivly(s2d::Sprite* sprite);

		/// <summary>
		/// This method handles the sprite in the Hirachy when its a parent. It will also search for a sprite in the Hirachy
		/// (Iterate recursivly over it) and open trees to see it highligted.
		/// -> gets called from 'displayChildsRecursivly'
		/// </summary>
		void displayTreeNode(s2d::Sprite* sprite, bool& popStyle);


		/// <summary>
		/// This method handles the sprite in the Hirachy when it doesnt have any childs. It will act the same as the the parent tho.
		/// -> gets called from 'displayChildsRecursivly'
		/// </summary>
		void displayMenuItem(s2d::Sprite* sprite, bool popStyle);

		/// <summary>
		/// This method handles the click events of 'displayMenuItem' and 'displayTreeNode'. It will check if the cursor got released on a (item) and then will set a 
		/// sprite which got previously selected - 'm_childSelectedToParent' to make a child of sprite.
		/// -> gets called in 'displayMenuItem' and 'displayTreeNode'
		/// </summary>
		///<param name="sprite"> The sprite which will have the child 'm_childSelectedToParent' </param>
		void childSystem(s2d::Sprite* sprite, bool isHeader);

		/// <summary>
		/// Deletes the sprite childs recursivly and changes its 'vecpos'
		/// -> gets called in 'deleteButton'
		/// </summary>
		///<param name="toDelete"> The parent which needs to be fully deletet </param>
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


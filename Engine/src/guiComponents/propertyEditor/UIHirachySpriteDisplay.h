#pragma once

#include <ImGui.h>
#include <physicalComponents/sprite.h>

namespace s2d
{
	class UIHirachy;
	class UIHirachySpriteDisplayer
	{
	private:
		ImVec4 m_spriteSelectedColor;

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

	public:

		s2d::Sprite* childSelectedToParent;
		s2d::Sprite* rightClickedSprite;

		UIHirachySpriteDisplayer();

		/// <summary>
		/// This method iterates over all sprites WHICH ARE PARENTS and it will call the 'displayChildsRecursivly' function. It second job is to 
		/// check if we clicked at no valid item, thenit should set the 'selectedSprite' to nullptr. This will display a default inspector view.
		/// -> Gets called in 'displayHirachyComponents' 
		/// </summary>
		void addSpritesToHirachy();

		void displayWindowWhenChildIsGettingDragged();
	};
}


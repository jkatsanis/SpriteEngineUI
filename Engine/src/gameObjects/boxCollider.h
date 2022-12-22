#pragma once

#include <SFML/Graphics.hpp>
#include <vector2.h>

namespace s2d
{
	class Sprite;
	class BoxCollider
	{
	public:
		//Gets set when we create a box collider in the UI inspector
		bool exists;
		bool isSolid;
		bool canCollide;

		//Deleting the pointer in gameEngine.cpp!
		s2d::Sprite* ptr_sprite;

		Vector2 boxColliderWidthLeftOrRight;
		Vector2 boxColliderHeightUpOrDown;

		BoxCollider();

		//Giving it a pointer, so we dont have to update it consistently
		BoxCollider(s2d::Sprite* ptr_sprite);
	
		void resetBoxCollider();
	};
}


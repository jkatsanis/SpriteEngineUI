#pragma once

#include <SFML/Graphics.hpp>
#include <calculationComponents/vector2.h>
#include <spriteComponents/component.h>

namespace s2d
{
	class Sprite;
	class BoxCollider : public s2d::Component
	{
	private:
		void init() override;
	public:
		bool isSolid;
		bool canCollide;

		//Deleting the pointer in gameEngine.cpp!
		s2d::Sprite* ptr_sprite;

		Vector2 boxColliderWidthLeftOrRight;
		Vector2 boxColliderHeightUpOrDown;

		BoxCollider();

		//Giving it a pointer, so we dont have to update it consistently
		BoxCollider(s2d::Sprite* ptr_sprite);
	
		void reset() override;
	};
}


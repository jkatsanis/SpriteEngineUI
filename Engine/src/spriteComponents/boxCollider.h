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
		bool is_solid;
		bool can_collide;

		//Deleting the pointer in gameEngine.cpp!
		s2d::Sprite* ptr_sprite;

		Vector2 box_collider_width;
		Vector2 box_collider_height;

		BoxCollider();
		BoxCollider(s2d::Sprite* sprite, s2d::BoxCollider& rhs);

		//Giving it a pointer, so we dont have to update it consistently
		BoxCollider(s2d::Sprite* ptr_sprite);
	
		void reset() override;
	};
}


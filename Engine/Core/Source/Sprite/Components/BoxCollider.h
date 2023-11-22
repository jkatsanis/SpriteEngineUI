#pragma once

#include <SFML/Graphics.hpp>

#include "Math/Vector2.h"
#include "Component.h"

namespace spe
{
	class Sprite;
	class BoxCollider : public spe::Component
	{
	private:
		void init() override;
	public:
		bool is_solid;
		bool can_collide;

		//Deleting the pointer in gameEngine.cpp!
		spe::Sprite* ptr_sprite;

		Vector2 box_collider_width;
		Vector2 box_collider_height;

		BoxCollider();
		BoxCollider(spe::Sprite* sprite, spe::BoxCollider& rhs);

		//Giving it a pointer, so we dont have to update it consistently
		BoxCollider(spe::Sprite* ptr_sprite);
	
		void reset() override;
	};
}


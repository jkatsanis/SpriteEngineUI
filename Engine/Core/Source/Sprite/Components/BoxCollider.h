#pragma once

#include <Include/SFML/Graphics.hpp>
#include <unordered_map>

#include "Math/Vector2.h"
#include "Sprite/Components/Component.h"

namespace spe
{
	class SpriteRepository;
	class Sprite;
	class BoxCollider : public spe::Component
	{
	private:
		int m_start = -5;
		int m_end = 5;

		bool m_got_left;
		bool m_got_right;
		bool m_got_up;
		bool m_got_down;

		void init() override;
		bool CheckCollision(spe::BoxCollider& other);
		void CheckCollisionPosition(spe::BoxCollider& other);

	public:
		bool is_solid;
		bool can_collide;
		bool collided_in_frame;
		std::unordered_map<uint32_t, spe::Sprite*> collided_sprite_map;

		bool left;
		bool right;
		bool up;
		bool down;
		bool collided;

		//Deleting the pointer in gameEngine.cpp!
		spe::Sprite* ptr_sprite;

		Vector2 box_collider_width;
		Vector2 box_collider_height;

		BoxCollider();
		BoxCollider(spe::Sprite* sprite, spe::BoxCollider& rhs);

		//Giving it a pointer, so we dont have to update it consistently
		BoxCollider(spe::Sprite* ptr_sprite);
	
		void reset() override;
		void Update(spe::SpriteRepository& tocheck);
	};
}


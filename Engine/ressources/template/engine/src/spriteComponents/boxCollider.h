#pragma once

#include <SFML/Graphics.hpp>
#include <calculationComponents/vector2.h>
#include <iostream>
#include <manager/spriteRepository.h>
#include <spriteComponents/component.h>

#include <unordered_map>

namespace s2d
{
	class Sprite;
	class BoxCollider : public s2d::Component
	{
	private:
		int m_start = -5;
		int m_end = 5;

		bool m_got_left;
		bool m_got_right;
		bool m_got_up;
		bool m_got_down;

		void checkPositions(const BoxCollider& other);
		void init() override;
	public:
		bool is_solid;
		bool can_collide;
		bool collided_in_frame;

		bool left;
		bool right;
		bool up;
		bool down;
		bool collided;

		s2d::Sprite* ptr_attached_sprite;
		std::unordered_map<uint32_t, s2d::Sprite*> collided_sprite_map;

		Vector2 box_collider_width;
		Vector2 box_collider_height;

		BoxCollider();

		//Giving it a pointer, so we dont have to update it consistently
		BoxCollider(s2d::Sprite* sprite);

		BoxCollider(s2d::Sprite* sprite, s2d::BoxCollider& rhs);

		void reset() override;

		void resetPositions();

		s2d::Sprite* collidedWithId(uint32_t id);
		s2d::Sprite* collidedWithTag(const std::string& tag);
		s2d::Sprite* collidedWithName(const std::string& name);

	public:
		static bool checkCollision(s2d::BoxCollider& lhs, s2d::BoxCollider& rhs);
		static void checkCollisions(s2d::SpriteRepository& spriteRepo);
	};
}


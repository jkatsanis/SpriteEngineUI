#pragma once

#include <SFML/Graphics.hpp>
#include <calculationComponents/vector2.h>
#include <spriteComponents/boxColliderPositionData.h>
#include <iostream>
#include <manager/spriteRepository.h>

namespace s2d
{
	class Sprite;
	class BoxCollider
	{
	private:
		static bool checkIAndJPCollisions(int i, int j, s2d::SpriteRepository& repo);

	private:
		int m_start = -5;
		int m_end = 5;

		void checkPositions(const BoxCollider& other, const int jIndex);

	public:
		int collision_cnt;
		bool is_solid;
		bool can_collide;
		bool exist;

		s2d::BoxColliderPositionData position_data;
		s2d::Sprite* sprite;
		s2d::Sprite* collidingSprite;

		Vector2 box_collider_width;
		Vector2 box_collider_height;

		BoxCollider();

		//Giving it a pointer, so we dont have to update it consistently
		BoxCollider(s2d::Sprite* sprite);

		//what is the j index? Eventually we collide we a object two times and so we need to push the object 
		//In two directions. When we We pushed it we reset the array and check for new collisions
		bool checkCollision(s2d::BoxCollider& other, const int jIndex);

		// MAY BE WRONG
		bool isInCollision() const
		{
			return this->position_data.isEqual(s2d::BoxColliderPositionData::Collision); 
		}

	public:
		static void checkCollisions(s2d::SpriteRepository& spriteRepo);
	};
}


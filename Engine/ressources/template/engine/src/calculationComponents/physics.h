#pragma once

#include <physicalComponents/sprite.h>
#include <engineComponents/sTime.h>

namespace s2d
{
	class Physics
	{
	public:
		
		Physics() = delete;

		static void update(s2d::SpriteRepository& repo);

		static void addForce(s2d::Sprite* sprite, const s2d::Vector2& direction, float force);
	};
}


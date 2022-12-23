#pragma once

#include <vector2.h>
#include <childSystem.h>

namespace s2d
{
	class Sprite;
	class Transform
	{
	public:
		s2d::Vector2 positionToParent;
		s2d::Vector2 position;
		s2d::Vector2 lastPos;
		s2d::Vector2 nextPos;
		s2d::Vector2 scale;
		s2d::Vector2 size;

		s2d::Vector2 l;
		s2d::Vector2 n;

		Transform();

	public:
		//Used in poll events
		static void onPositionChange(s2d::Sprite* sprite);
	};
}


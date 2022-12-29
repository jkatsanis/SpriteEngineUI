#pragma once

#include <vector2.h>
#include <childSystem.h>

namespace s2d
{
	class Sprite;
	class Transform 
	{
	public:
		s2d::Vector2 position;
		s2d::Vector2 scale;
		s2d::Vector2 nextPos;
		s2d::Vector2 lastPos;
		s2d::Vector2 positionToParent;

		Transform();


		/// <summary>
		/// This method sets the last position of the Transform
		/// </summary>
		/// <returns>True if it set the last position succesful</returns>
		bool setLastPosition();

	public:
		//Used in poll events
		static void onPositionChange(s2d::Sprite* sprite);
	};
}


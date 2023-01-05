#pragma once

#include <calculationComponents/vector2.h>

namespace s2d
{
	class PhsysicsBody
	{
	private:

	public:

		float mass;
		float gravity;
		bool exists;
		s2d::Vector2 velocity;

		PhsysicsBody();

		void resetPhysicsBody();
	};
}
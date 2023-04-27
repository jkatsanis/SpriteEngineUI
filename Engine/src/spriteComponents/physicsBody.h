#pragma once

#include <calculationComponents/vector2.h>
#include <spriteComponents/component.h>

namespace s2d
{
	class PhsysicsBody : public s2d::Component
	{
	private:
		void init() override;
	public:
		float mass;
		float gravity;
		s2d::Vector2 velocity;

		PhsysicsBody();

		void reset() override;
	};
}
#pragma once

#include <calculationComponents/vector2.h>
#include <engineComponents/sTime.h>
#include <spriteComponents/component.h>

namespace s2d
{
	class Sprite;
	class PhsysicsBody : public s2d::Component
	{
	private:
		void init() override;

	public:
		float mass;
		float gravity;
		s2d::Vector2 velocity;

		s2d::Sprite* ptr_attachedSprite;

		PhsysicsBody();
		PhsysicsBody(s2d::Sprite* ptr_attachedSprite);
		PhsysicsBody(s2d::Sprite* attached, const s2d::PhsysicsBody& rhs);

		void reset() override;

		void fixedUpdate();
	};
}
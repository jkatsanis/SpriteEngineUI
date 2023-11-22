#pragma once

#include "Math/Vector2.h"
#include "Component.h"

namespace spe
{
	class PhsysicsBody : public spe::Component
	{
	private:
		void init() override;
	public:
		float mass;
		float gravity;
		spe::Vector2 velocity;

		PhsysicsBody();
		PhsysicsBody(const spe::PhsysicsBody& rhs);

		void reset() override;
	};
}
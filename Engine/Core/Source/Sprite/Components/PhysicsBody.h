#pragma once

#include "Core/Time.h"
#include "Math/Vector2.h"
#include "Sprite/Components/Component.h"

namespace spe
{
	class Sprite;
	class PhsysicsBody : public spe::Component
	{
	private:
		void init() override;
	public:
		float mass;
		float gravity;
		spe::Vector2 velocity;
		spe::Sprite* ptr_Sprite;

		PhsysicsBody();
		PhsysicsBody(spe::Sprite* spr);
		PhsysicsBody(spe::Sprite* spr, const spe::PhsysicsBody& rhs);

		void reset() override;
		void Update();
	};
}
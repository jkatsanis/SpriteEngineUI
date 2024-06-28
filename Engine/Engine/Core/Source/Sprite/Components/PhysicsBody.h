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
		void Init() override;
	public:
		float Mass;
		float Gravity;
		spe::Vector2 Velocity;
		spe::Sprite* ptr_Sprite;

		PhsysicsBody();
		PhsysicsBody(spe::Sprite* spr);
		PhsysicsBody(spe::Sprite* spr, const spe::PhsysicsBody& rhs);

		void Reset() override;
		void Update();
	};
}
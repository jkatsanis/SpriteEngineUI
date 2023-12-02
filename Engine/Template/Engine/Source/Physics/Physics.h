#pragma once

#include "Sprite/Sprite.h"

namespace spe
{
	class Physics
	{
	public:

		Physics() = delete;

		static void AddForce(spe::Sprite* sprite, const spe::Vector2& direction, float force);
	};
}


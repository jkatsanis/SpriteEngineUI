#pragma once

#include "Sprite/Sprite.h"

namespace spe
{
	class Physics
	{
	public:

		Physics() = delete;

		static void AddForce(spe::Sprite* sprite, const spe::Vector2& direction, float force);

		static float Lerp(float a, float b, float t);

		static spe::Vector2 Lerp(const spe::Vector2& a, const spe::Vector2& b, float t);
	};
}


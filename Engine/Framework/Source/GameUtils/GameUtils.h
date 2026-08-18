#pragma once

#include "Sprite/Sprite.h"
#include <random>

namespace spe
{
	class GameUtils
	{
	public:

		GameUtils() = delete;

		static bool IsLeft(spe::Sprite* maybe_left, spe::Sprite* maybe_right);
		static int32_t Random(int32_t mix, int32_t max);
		static bool IsInRangeX(const spe::Sprite* sprite1, const spe::Sprite* sprite2, int range);
	};
}


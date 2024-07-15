#pragma once

#include "Sprite/Sprite.h"

namespace spe
{
	class GameUtils
	{
	public:

		GameUtils() = delete;

		static bool IsLeft(spe::Sprite* maybe_left, spe::Sprite* maybe_right);

	};
}


#pragma once

#include <physicalComponents/sprite.h>

namespace s2d
{
	class Initializer
	{
	public:
		Initializer() = delete;

		static void initAnimations(std::vector<s2d::Sprite*> to);
	};
}


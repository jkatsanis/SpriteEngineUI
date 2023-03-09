#pragma once

#include <physicalComponents/sprite.h>
#include <spriteComponents/animator.h>
#include <fstream>

namespace s2d
{
	class Initializer
	{
	public:
		Initializer() = delete;

		static void initAnimations(std::vector<s2d::Sprite*> to);
	};
}


#pragma once

#include <cstdlib>
#include <random>

namespace s2d
{
	class Random
	{
	public:
		Random() = delete;

		static int getRandomNumber(int from, int to);
	};
}


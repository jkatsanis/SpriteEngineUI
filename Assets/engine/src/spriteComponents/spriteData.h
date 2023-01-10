#pragma once

#include <iostream>
#include <fstream>
#include <string>

namespace s2d
{
	class SpriteData
	{
		static int getHighestIndex();

	public:
		SpriteData() = delete;

		static int highestSpriteID;
	};
}


#pragma once

#include <iostream>
#include <fstream>
#include <string>

namespace s2d
{
	class SpriteData
	{
	public:
		SpriteData() = delete;

		static int getHighestIndex();

		static std::string defaultSpritePath;
		static int highestIndex;
		static int dupeNameCounter;
	};
}


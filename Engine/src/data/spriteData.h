
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <data/engineData.h>
#include <manager/initializer.h>

namespace s2d
{
	class SpriteData
	{
	public:
		SpriteData() = delete;

		static int getHighestIndex();

		static std::string defaultSpritePath;
		static int highestSpriteID;
		static int dupeNameCounter;
	};
}


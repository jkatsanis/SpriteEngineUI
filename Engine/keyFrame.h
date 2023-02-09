#pragma once

#include <iostream>

namespace s2d
{
	struct KeyFrame
	{
		std::string path;
		short delay;

		KeyFrame(const std::string& path, short delay)
		{
			this->path = path;
			this->delay = delay;
		}
	};
}


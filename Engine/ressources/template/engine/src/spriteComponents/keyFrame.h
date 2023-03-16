#pragma once

#include <iostream>

namespace s2d
{
	struct KeyFrame
	{
		std::string path;
		int delay;
		int position;

		KeyFrame() { this->path = ""; this->delay = 0; }
		KeyFrame(const std::string& path, const int delay)
		{
			this->position = -1;
			this->path = path;
			this->delay = delay;
		}
	};
}


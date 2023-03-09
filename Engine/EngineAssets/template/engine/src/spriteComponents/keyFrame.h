#pragma once

#include <iostream>

namespace s2d
{
	struct KeyFrame
	{
		std::string path;
		float delay;
		int position;

		KeyFrame() { this->path = ""; this->delay = 0.0f; }
		KeyFrame(const std::string& path, const float delay)
		{
			this->position = -1;
			this->path = path;
			this->delay = delay;
		}
	};
}


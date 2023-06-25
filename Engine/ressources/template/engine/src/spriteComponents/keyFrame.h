#pragma once

#include <iostream>

namespace s2d
{
	struct KeyFrame
	{
		std::string path;
		float delay;
		int32_t position;

		KeyFrame() { this->path = ""; this->delay = 0; }
		KeyFrame(const std::string& path, const float delay)
		{
			this->position = -1;
			this->path = path;
			this->delay = delay;
		}

		KeyFrame(const s2d::KeyFrame& frame)
		{
			this->position = -1;
			this->path = frame.path;
			this->delay = frame.delay;
		}
	};
}


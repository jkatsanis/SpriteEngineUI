#pragma once

#include <iostream>

namespace spe
{
	struct KeyFrame
	{  
		std::string path;
		float delay;
		uint32_t position;

		KeyFrame() { this->path = ""; this->delay = 0; this->position = 0; }
		KeyFrame(const std::string& path, const float delay)
		{
			this->position = 0;
			this->path = path;
			this->delay = delay;
		}
	};
}


#pragma once

#include <iostream>
#include  "UtilityFunctions.h"

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
			const std::string updatetPath = spe::Utility::ToRightPath(path);

			this->position = 0;
			this->path = updatetPath;
			this->delay = delay;
		}
	};
}


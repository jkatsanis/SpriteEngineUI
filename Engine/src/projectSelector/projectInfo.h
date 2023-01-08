#pragma once

#include <iostream>

namespace s2d
{
	struct ProjectInfo
	{
		std::string path;
		std::string name;
		std::string lastOpened;

		ProjectInfo()
		{
			this->lastOpened = "none";
			this->path = "none";
			this->name = "none";
		}
		ProjectInfo(std::string name, std::string path, std::string lastOpened)
		{
			this->lastOpened = lastOpened;
			this->name = name;
			this->path = path;
		}
	};
}


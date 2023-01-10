#pragma once

#include <iostream>
#include <manager/fileDialogs.h>
#include <direct.h>

namespace s2d
{
	enum class UIProjectSelectorLocation
	{
		None = -1,
		Projects = 0,
		Docs
	};
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

		static std::string s_pathToUserProject;
		
		static std::string getSolutionDir();
	};

}


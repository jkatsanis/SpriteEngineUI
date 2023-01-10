#pragma once

#include <iostream>
#include <manager/fileDialogs.h>

namespace s2d
{
	enum class UIProjectSelectorLocation
	{
		None = -1,
		Projects = 0,
		Docs
	};
	struct UserProjectInfo
	{
		std::string absulutePath;

		/// <summary>
		/// Engine to user project
		/// </summary>
		std::string relativePath;
		std::string name;
		std::string lastOpened;

		UserProjectInfo()
		{
			this->lastOpened = "none";
			this->absulutePath = "none";
			this->name = "none";
		}
		UserProjectInfo(std::string name, std::string abspath, std::string lastOpened, std::string relativePath)
		{
			this->lastOpened = lastOpened;
			this->name = name;
			this->absulutePath = abspath;
			this->relativePath = relativePath;

		}
	};
}


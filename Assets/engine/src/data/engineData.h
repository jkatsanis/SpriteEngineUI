#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <stdExtension.h>

namespace s2d
{
	class EngineData
	{
		static std::string initPathToEngine();

	public:
		EngineData() = delete;

		static std::string s_pathFromEngineToUserProject;
	};
}


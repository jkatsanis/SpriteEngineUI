#pragma once

#include <iostream>

namespace s2d
{
	class EngineData
	{
	public:
		EngineData() = delete;

		static std::string s_pathToUserProject;
		static std::string s_nameOfUserProject;
	};
}


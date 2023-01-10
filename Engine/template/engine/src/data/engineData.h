#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <stdExtension.h>

namespace s2d
{
	class EngineData
	{
	private:

		static std::string getPathToEngine();

	public:
		EngineData() = delete;

		static std::string s_pathToEngine;
	};
}


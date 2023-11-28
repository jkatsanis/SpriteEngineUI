#pragma once

#include <iostream>

#include "UtilityFunctions.h"

namespace spe
{
	class EngineData
	{
	public:
		EngineData() = delete;

		static void BuildProjectFiles();
		static void BuildProject();

		static std::string s_PathOfEngine;
		static std::string s_PathUserProject;
		static std::string s_NameOfUser;
		static std::string s_Scene;

		static bool s_BuildDebug;
		static bool s_BuildRelease;
	};
}


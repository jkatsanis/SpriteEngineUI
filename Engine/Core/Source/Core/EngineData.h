#pragma once

#include <iostream>

namespace spe
{
	class EngineData
	{
	public:
		EngineData() = delete;

		static std::string s_PathUserProject;
		static std::string s_name_of_user_project;
		static std::string s_Scene;
	};
}


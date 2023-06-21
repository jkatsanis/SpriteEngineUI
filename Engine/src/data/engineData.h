#pragma once

#include <iostream>

namespace s2d
{
	class EngineData
	{
	public:
		EngineData() = delete;

		static std::string s_path_to_user_project;
		static std::string s_name_of_user_project;
		static std::string s_scene;
	};
}


#pragma once

#include <iostream>

#define PATH_TO_SPRITE_FILE s2d::EngineData::s_pathToUserProject + "\\saves\\sprites.txt"
#define PATH_TO_ENGINE_TO_USER_FILE s2d::EngineData::s_pathToUserProject + "\\saves\\pathEngineToUser.txt"

namespace s2d
{
	class EngineData
	{
	public:
		EngineData() = delete;

		static std::string s_pathToUserProject;
	};
}


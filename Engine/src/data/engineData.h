#pragma once

#include <iostream>

#define PATH_TO_SPRITE_FILE s2d::EngineData::s_pathToUserProject + "\\engine\\saves\\sprites.txt"
#define PATH_TO_BACKGROUND_FILE s2d::EngineData::s_pathToUserProject + "\\engine\\saves\\background.txt"
#define PATH_TO_CAMERA_FILE s2d::EngineData::s_pathToUserProject + "\\engine\\saves\\camera.txt"
#define PATH_TO_INDEX_FILE s2d::EngineData::s_pathToUserProject + "\\engine\\saves\\index.txt"

namespace s2d
{
	class EngineData
	{
	public:
		EngineData() = delete;

		static std::string s_pathToUserProject;
	};
}


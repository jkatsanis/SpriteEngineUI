#pragma once

#include <physicalComponents/sprite.h>
#include <fstream>
#include <vector>
#include <calculationComponents/vector3.h>
#include <physicalComponents/camera.h>
#include <dirent.h>
#include <filesystem>

namespace s2d
{
	namespace flc
	{
		void createSaveFile(std::vector<s2d::Sprite*>& sprite);
		void createWindowBackgroundSaveFile(const s2d::Vector3 windowBackground);
		void createCameraSaveFile(const s2d::Camera& camera);
		void createIndexSaveFile();

		/// <summary>
		/// Creates a file in the user project which has a path to the engine. 
		/// Only relevant for user project
		/// </summary>
		void createPathToEngineFile();

		void copyDir(const char* inputDir, std::string outputDir, std::string name);

		std::string getPropertyLineWithSeperator(Sprite* sprite);
	}
}



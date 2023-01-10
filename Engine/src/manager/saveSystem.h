#pragma once

#include <physicalComponents/sprite.h>
#include <fstream>
#include <vector>
#include <calculationComponents/vector3.h>
#include <physicalComponents/camera.h>
#include <data/engineData.h>

namespace s2d
{
	namespace flc
	{
		void createSaveFile(std::vector<s2d::Sprite*>& sprite);
		void createWindowBackgroundSaveFile(const s2d::Vector3 windowBackground);
		void createCameraSaveFile(const s2d::Camera& camera);
		void createIndexSaveFile();

		/// <summary>
		/// This file needs to be rewritten when the engine gets moved. 
		/// The project wont show in the selector then aswell you need to 
		/// manually open it
		/// </summary>
		void createPathFromEngineToUserProjectSaveFile();

		std::string getPropertyLineWithSeperator(Sprite* sprite);
	}
}



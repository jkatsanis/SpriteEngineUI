#pragma once

#include <physicalComponents/sprite.h>
#include <fstream>
#include <vector>
#include <calculationComponents/vector3.h>
#include <physicalComponents/camera.h>
#include <data/engineData.h>	

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

namespace s2d
{
	namespace flc
	{
		void createSaveFile(std::vector<s2d::Sprite*>& sprite);
		void createWindowBackgroundSaveFile(const s2d::Vector3 windowBackground);
		void createCameraSaveFile(const s2d::Camera& camera);
		void createIndexSaveFile();
		void createKnownProjectDirFile();

		bool checkIfProjectExistInFile(std::string& ref);
		std::string getAbsulutPath();
		std::string getDate();

		std::string getPropertyLineWithSeperator(Sprite* sprite);
	}
}



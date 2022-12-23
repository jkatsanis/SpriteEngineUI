#pragma once

#include <sprite.h>
#include <fstream>
#include <vector>
#include <vector3.h>
#include <camera.h>

namespace s2d
{
	namespace flc
	{
		void createSaveFile(std::vector<s2d::Sprite*>& sprite);
		void createWindowBackgroundSaveFile(const s2d::Vector3 windowBackground);
		void createCameraSaveFile(const s2d::Camera& camera);
		void createIndexSaveFile();

		std::string getPropertyLineWithSeperator(Sprite* sprite);
	}
}



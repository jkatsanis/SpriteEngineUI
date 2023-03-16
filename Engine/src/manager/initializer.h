#pragma once

#include <physicalComponents/sprite.h>
#include <spriteComponents/animator.h>
#include <physicalComponents/sprite.h>
#include <guiComponents/UI.h>

#include <string>
#include <fstream>

#define DELIMITER ";"

#define EXTENSION_SAVE_FILE ".txt"
#define EXTENSION_ANIMATION_FILE ".txt"
#define PATH_TO_RESSOURCS "ressources"

#define PATH_TO_ANIMATION_FILE s2d::EngineData::s_pathToUserProject + "\\engine\\saves\\animation.txt"
#define PATH_TO_SPRITE_FILE s2d::EngineData::s_pathToUserProject + "\\engine\\saves\\sprites.txt"
#define PATH_TO_BACKGROUND_FILE s2d::EngineData::s_pathToUserProject + "\\engine\\saves\\background.txt"
#define PATH_TO_CAMERA_FILE s2d::EngineData::s_pathToUserProject + "\\engine\\saves\\camera.txt"
#define PATH_TO_INDEX_FILE s2d::EngineData::s_pathToUserProject + "\\engine\\saves\\index.txt"
#define PATH_TO_KNOWN_PROJECTS  PATH_TO_RESSOURCS"\\saves\\projects.txt" 

#define PATH_TO_KNOWN_ANIMATIONS s2d::EngineData::s_pathToUserProject + "\\engine\\saves\\animations.txt"

namespace s2d
{
	class Initializer
	{
	private:
		static void initAnimation(const std::string& path);
	public:
		Initializer() = delete;

		static void initAnimations();
		static void initSprites();
	};
}


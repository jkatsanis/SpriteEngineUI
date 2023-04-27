#pragma once

#include <physicalComponents/sprite.h>
#include <spriteComponents/animator.h>
#include <physicalComponents/sprite.h>
#include <guiComponents/UI.h>
#include <calculationComponents/vector3.h>
#include <manager/spriteRepository.h>
#include <string>
#include <fstream>

#define DELIMITER ";"

#define EXTENSION_PREFAB_FILE ".prfb"
#define EXTENSION_SAVE_FILE ".txt"
#define EXTENSION_ANIMATION_FILE ".txt"
#define PATH_TO_RESSOURCS "ressources"

#define PATH_TO_USER_ASSET_FOLDER s2d::EngineData::s_path_to_user_project + "\\assets"

#define PATH_TO_USER_DEBUG_FOLDER s2d::EngineData::s_path_to_user_project + "\\x64\\Debug\\" 

#define PATH_TO_ANIMATION_FILE s2d::EngineData::s_path_to_user_project + "\\engine\\saves\\animation.txt"
#define PATH_TO_SPRITE_FILE s2d::EngineData::s_path_to_user_project + "\\engine\\saves\\sprites.txt"
#define PATH_TO_BACKGROUND_FILE s2d::EngineData::s_path_to_user_project + "\\engine\\saves\\background.txt"
#define PATH_TO_CAMERA_FILE s2d::EngineData::s_path_to_user_project + "\\engine\\saves\\camera.txt"
#define PATH_TO_INDEX_FILE s2d::EngineData::s_path_to_user_project + "\\engine\\saves\\index.txt"
#define PATH_TO_KNOWN_PROJECTS  PATH_TO_RESSOURCS"\\saves\\projects.txt" 

#define PATH_TO_KNOWN_ANIMATIONS s2d::EngineData::s_path_to_user_project + "\\engine\\saves\\animations.txt"

namespace s2d
{
	class Initializer
	{
	private:
		static void initAnimation(const std::string& path, s2d::SpriteRepository& repo);
	public:
		Initializer() = delete;

		static void initAnimations(s2d::SpriteRepository& repo);
		static void initSprite(const std::string& line, s2d::Sprite* sprite);
		static void initSprites(s2d::SpriteRepository& spriteRepo);
		static void initBackground(s2d::Vector3& vec);
		static void initIds(uint32_t& highestId);
	};
}


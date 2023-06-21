#pragma once

#include <physicalComponents/sprite.h>
#include <spriteComponents/animator.h>
#include <physicalComponents/sprite.h>
#include <guiComponents/UI.h>
#include <calculationComponents/vector3.h>
#include <manager/spriteRepository.h>
#include <string>
#include <fstream>
#include <manager/guiRepository.h>
#include <data/fileDataMacros.h>

namespace s2d
{
	class Initializer
	{
	private:
		static void initAnimation(const std::string& path, s2d::SpriteRepository& repo, int32_t idx);
	public:
		Initializer() = delete;

		static void initScenes(std::vector<std::string>& scenes);
		static void initPrefab(const std::string& path, s2d::SpriteRepository& repo);
		static void initCamera(s2d::GUIRepository& repo);
		static void initAnimations(s2d::SpriteRepository& repo);
		static void initSprite(const std::string& line, s2d::Sprite* sprite);
		static void initSprites(s2d::SpriteRepository& spriteRepo);
		static void initBackground(s2d::Vector3& vec);
		static void initIds(uint32_t& highestId);
	};
}


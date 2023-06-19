#pragma once

#include <physicalComponents/sprite.h>
#include <spriteComponents/animator.h>
#include <physicalComponents/sprite.h>
#include <data/fileDataMacros.h>
#include <manager/prefabRepository.h>

#include <string>
#include <fstream>

namespace s2d
{
	class Initializer
	{
	private:
		static void initPrefab(const std::string& path);
		static void initAnimation(const std::string& path, s2d::SpriteRepository& repo);
		static void initAnimation(const std::string& path, s2d::Sprite* spr);

	public:
		Initializer() = delete;
		static void initAnimations(SpriteRepository& repo);
		static void loadPrefabsInMemory();
		static void initSprites(s2d::SpriteRepository& repo);
		static void initIds(uint32_t& highestId);
		static void initSprite(const std::string& line, s2d::Sprite* sprite);
	};
}


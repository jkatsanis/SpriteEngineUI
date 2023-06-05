#pragma once

#include <physicalComponents/sprite.h>
#include <spriteComponents/animator.h>
#include <physicalComponents/sprite.h>

#include <string>
#include <fstream>

#define DELIMITER ";"

#define EXTENSION_SAVE_FILE ".txt"

#define PATH_TO_KNOWN_ANIMATIONS "engine\\saves\\animations.txt"
#define PATH_TO_SPRITES "engine\\saves\\sprites.txt"


namespace s2d
{
	class Initializer
	{
	private:
		static void initAnimation(const std::string& path, s2d::SpriteRepository& repo);
	public:
		Initializer() = delete;

		static void initAnimations(s2d::SpriteRepository& repo);
		static void initSprites(s2d::SpriteRepository& repo);
		static void initIds(uint32_t& highestId);
	};
}


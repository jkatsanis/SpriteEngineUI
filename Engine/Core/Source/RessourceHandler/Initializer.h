#pragma once

#include "SpriteRepository.h"
#include "Sprite.h"

#include <string>
#include <fstream>

#define DELTIMITER ';'

namespace spe
{
	class Initializer
	{
	public:
		Initializer() = delete;

		static void initSprite(const std::string& line, spe::Sprite* sprite);
		static void initSprites(spe::SpriteRepository& spriteRepo, const std::string& path);

	};
}


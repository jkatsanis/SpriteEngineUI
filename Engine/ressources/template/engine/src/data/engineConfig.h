#pragma once

#include <manager/spriteRepository.h>

namespace s2d
{
	class GameEngine;
	class EngineConfig 
	{
	private:
		s2d::GameEngine* m_ptr_game_engine;

	public:
		s2d::SpriteRepository* ptr_sprites;

		void loadScene(const std::string& name);
	};
}
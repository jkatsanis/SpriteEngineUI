#pragma once

#include <SFML/Graphics.hpp>
#include <calculationComponents/vector3.h>
#include <imgui-SFML.h>
#include <imgui.h>
#include <physicalComponents/sprite.h>
#include <data/FileData.h>
#include <manager/spriteRepository.h>

//#define UPDATE_SPRITE_TEXTURES_CONSISTENT 

namespace s2d
{
	class Renderer
	{
	private:
		float m_timePassedToUpdateLayerIndex;
		s2d::SpriteRepository* m_ptr_repo;
		uint8_t  m_timeToUpdateLayerIndex;
		uint8_t m_timeToUpdateSpriteTexture;

		sf::RenderWindow* m_ptr_renderWindow;

		void draw();

		void drawRectangles();
		void drawSprites();

	public:
		Renderer();
		Renderer(sf::RenderWindow* renderWindow);

		void render();

		void setSpriteRepository(s2d::SpriteRepository& repo) { this->m_ptr_repo = &repo; }
	public:

		/// <summary>
		/// UPDATES ALL sprite textures from the files (very inefficent)
		/// </summary>
		static void updateAllSpriteTextures(s2d::SpriteRepository& repo);
	};
}


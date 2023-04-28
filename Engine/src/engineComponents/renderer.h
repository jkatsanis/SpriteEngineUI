#pragma once

#include <SFML/Graphics.hpp>
#include <calculationComponents/vector3.h>
#include <imgui-SFML.h>
#include <imgui.h>
#include <physicalComponents/sprite.h>
#include <manager/spriteRepository.h>
#include <manager/guiRepository.h>

// #define LOAD_TEXTURE_FROM_FILES

namespace s2d
{
	class Renderer
	{
	private:
	    float m_timePassedToUpdateLayerIndex;
	    float m_timePassedTillNextSpriteTextureUpdate;

		unsigned short  m_timeToUpdateLayerIndex;
		unsigned short m_timeToUpdateSpriteTexture;

		sf::RenderWindow* m_ptr_render_window;
		const s2d::Vector3* m_ptr_background_color;
		s2d::SpriteRepository* m_ptr_sprite_repo;
		s2d::GUIRepository* m_ptr_gui_repo;

		void draw();

		void drawLines();
		void drawRectangles();
		void drawSprites();
		void updateSriteTextures();

	public:
		Renderer();
		Renderer(sf::RenderWindow* renderWindow, const s2d::Vector3* bg, s2d::SpriteRepository& spriteRepo, s2d::GUIRepository& repo);

	    void render();
	};
}


#pragma once

#include <SFML/Graphics.hpp>
#include <calculationComponents/vector3.h>
#include <imgui-SFML.h>
#include <imgui.h>
#include <physicalComponents/sprite.h>
#include <manager/spriteRepository.h>
#include <manager/guiRepository.h>
#include <manager/lightRepository.h>

namespace s2d
{
	class Renderer
	{
	private:
	    float m_timePassedToUpdateLayerIndex;

		uint16_t m_timeToUpdateLayerIndex;

		sf::RenderWindow* m_ptr_render_window;
		s2d::SpriteRepository* m_ptr_sprite_repo;
		s2d::GUIRepository* m_ptr_gui_repo;

		void draw();

		void drawLines();
		void drawRectangles();
		void drawSprites();

	public:
		Renderer();
		Renderer(sf::RenderWindow* renderWindow, s2d::SpriteRepository& spriteRepo, s2d::GUIRepository& repo);

	    void render();
	};
}


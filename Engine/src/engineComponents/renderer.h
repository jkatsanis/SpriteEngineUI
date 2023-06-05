#pragma once

#include <SFML/Graphics.hpp>
#include <calculationComponents/vector3.h>
#include <imgui-SFML.h>
#include <imgui.h>
#include <physicalComponents/sprite.h>

//s#define LOAD_TEXTURE_FROM_FILES

namespace s2d
{
	class Renderer
	{
	private:
	    float m_timePassedToUpdateLayerIndex;
	    float m_timePassedTillNextSpriteTextureUpdate;

		unsigned short  m_timeToUpdateLayerIndex;
		unsigned short m_timeToUpdateSpriteTexture;

		sf::RenderWindow* m_ptr_renderWindow;
		const s2d::Vector3* m_ptr_backGroundColor;

		void draw();

		void drawLines();
		void drawRectangles();
		void drawSprites();
		void updateSriteTextures();

	public:
		Renderer();
		Renderer(sf::RenderWindow* renderWindow, const s2d::Vector3* bg);

	   void render();
	};
}


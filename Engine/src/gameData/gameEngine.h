#pragma once

#include "SFML/Graphics.hpp"
#include <UIWindow.h>
#include <UIRealTimeEditor.h>
#include <imgui-SFML.h>
#include <imgui.h>
#include <stime.h>
#include <input.h>
#include <vector>
#include <event.h>

namespace s2d
{
	class GameEngine
	{
	private:
		UIWindow m_UIWindow;
		UIRealTimeEditor m_UIRealTimeEditor;

		bool m_isWindowFullScreen;
		float m_timePassed;

		void pollEngineEvents();
		void pollEvents();
		void render();
		void initFonts();
		void draw();
		void drawSprites();
		void drawLines();
		void drawRectangles();
		void updateWindowStyle();

	public:
		s2d::Event event;
		sf::Event windowEvent;
		std::unique_ptr<sf::RenderWindow> ptr_renderWindow;

		GameEngine();

		//Deleting all the sprites here since when we close the engine we dont need anymore
		~GameEngine();

		void update();	

	};
}


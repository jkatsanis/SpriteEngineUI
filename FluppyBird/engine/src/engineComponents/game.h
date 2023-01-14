#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <engineComponents/input.h>
#include <engineComponents/sTime.h>
#include <data/FileData.h>
#include <calculationComponents/physics.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <manager/GUIManager.h>

//All user files
#include "Testing.h"

namespace s2d
{
	class Game
	{
		//User classes, only 1 instance
		Testing testing;

		float m_timePassed;

		void pollEngineEvents();
		void pollEvents();
		void render();
		void updateUserScriptsAndGUI();

		void draw();
		void drawSprites();

	public:
		s2d::Event event;
		sf::Event windowEvent;
		std::unique_ptr<sf::RenderWindow> ptr_renderWindow;

		Game();
		~Game();

		void update();
		void start();
		bool isGameRunning();
	};
}


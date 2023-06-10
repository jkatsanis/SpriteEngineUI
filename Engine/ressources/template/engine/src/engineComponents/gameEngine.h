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
#include <engineComponents/renderer.h>
#include <features.h>
#include <manager/initializer.h>
#include <manager/spriteRepository.h>

//All user files

#include "game.h"

namespace s2d
{
    class GameEngine
	{
		//User classes, only 1 instance
		Game m_game;

		s2d::SpriteRepository m_sprite_repository;
		s2d::Renderer m_renderer;

		float m_timePassed;
		bool m_isWindowFullScreen;

		void pollEngineEvents();
		void pollEvents();
		void updateUserScriptsAndGUI();
		void updateWindowStyle();

	public:
		s2d::Event event;
		sf::Event windowEvent;
		sf::RenderWindow* ptr_render_window;

		GameEngine();
		~GameEngine();

		void update();
		void start();
		bool isGameRunning();
	};
}


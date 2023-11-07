#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <engineComponents/input.h>
#include <engineComponents/sTime.h>
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
		std::vector<std::string> m_scene_names;
		std::string m_current_scene;

		s2d::SpriteRepository m_sprite_repository;
		s2d::Renderer m_renderer;

		float m_timePassed;
		bool m_isWindowFullScreen;

		void pollEngineEvents();
		void pollEvents();
		void updateUserScriptsAndGUI();
		void updateWindowStyle();
		void clearEngineUpBeforeSceneLoad();
		void initOtherClasses();
		void updateComponents();

	public:
		s2d::Event event;
		sf::Event windowEvent;
		sf::RenderWindow* ptr_render_window;

		GameEngine();
		~GameEngine();

		void loadScene(const std::string& scene_name);
		void update();
		void start();
		bool isGameRunning();
	};
}


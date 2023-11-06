#pragma once

#include "SFML/Graphics.hpp"
#include <guiComponents/UIWindow.h>
#include <UIRealTimeEditor.h>
#include <imgui-SFML.h>
#include <imgui.h>
#include <engineComponents/stime.h>
#include <engineComponents/input.h>
#include <vector>
#include <engineComponents/event.h>
#include <engineComponents/renderer.h>
#include <guiComponents/UI.h>
#include <manager/initializer.h>
#include <manager/spriteRepository.h>
#include <manager/lightRepository.h>

namespace s2d
{
	class GameEngine
	{
	private:
		UIWindow m_ui_window;
		UIRealTimeEditor m_ui_real_time_editor;
		Renderer m_renderer;
		SpriteRepository m_sprite_repository;
		std::string m_current_scene;
		std::vector<std::string> m_scene_names;

		bool m_is_window_full_screen;
		bool m_close;

		void pollEngineEvents();
		void pollEvents();
		void updateWindowStyle();
		void saveDialoge();
		void onEngineClose(bool save);
		void clearEngineUpBeforeSceneLoad();
		void initOtherClasses();
		void loadScene(const std::string& scene_name);
		void updateComponents();

	public:

		s2d::Event event;
		sf::Event windowEvent;
		sf::RenderWindow* ptr_render_window;

		GameEngine();

		//Deleting all the sprites here since when we close the engine we dont need anymore
		~GameEngine();

		void update();

		s2d::SpriteRepository& getRepo() {
			return this->m_sprite_repository;
		}
	};
}


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

namespace s2d
{
	class GameEngine
	{
	private:
		UIWindow m_ui_window;
		UIRealTimeEditor m_ui_real_time_editor;
		Renderer m_renderer;
		SpriteRepository m_sprite_repository;

		bool m_isWindowFullScreen;
		bool m_close;

		void pollEngineEvents();
		void pollEvents();
		void updateWindowStyle();
		void saveDialoge();
		void onEngineClose(bool save);

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


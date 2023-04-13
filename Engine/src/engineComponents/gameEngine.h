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
		UIWindow m_UIWindow;
		UIRealTimeEditor m_UIRealTimeEditor;
		Renderer m_renderer;
		SpriteRepository m_spriteRepository;

		bool m_isWindowFullScreen;
		bool m_close;

		void pollEngineEvents();
		void pollEvents();
		void updateWindowStyle();
		void saveDialoge();

	public:
		s2d::Event event;
		sf::Event windowEvent;
		sf::RenderWindow* ptr_renderWindow;

		GameEngine();

		//Deleting all the sprites here since when we close the engine we dont need anymore
		~GameEngine();

		void update();

	};
}


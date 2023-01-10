#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <engineComponents/stime.h>
#include <manager/fontManager.h>
#include <icons.h>
#include <guiComponents/UIWindow.h>
#include <UIProjectSelectorProjectSection.h>
#include <projectSelectorColors.h>

namespace s2d
{
	class UIProjectSelector
	{
	private:
		sf::RenderWindow* m_ptr_toRenderWindow;
		sf::Event m_renderWindowEvents;

		s2d::UIProjectSelectorProjectSection m_projectSection;
		s2d::UIProjectSelectorLocation m_userLocation;

		void pollEvents();
		void render();

		void docsData();

		void renderProjectDataOrDocs();
			
		s2d::UIProjectSelectorLocation getUserInputForDataToRender();


	public:
		s2d::ProjectInfo project;

		UIProjectSelector();
		~UIProjectSelector();

		void update();

	public:
		bool isOpen() { return this->m_ptr_toRenderWindow->isOpen(); }
	};
}


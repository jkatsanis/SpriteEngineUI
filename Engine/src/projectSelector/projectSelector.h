#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <engineComponents/stime.h>
#include <manager/fontManager.h>
#include <icons.h>
#include <guiComponents/UIWindow.h>
#include <projectSelector/projectInfo.h>

namespace s2d
{
	enum class Location
	{
		None = -1,
		Projects = 0,
		Docs
	};
	class ProjectSelector
	{
	private:
		sf::RenderWindow* m_ptr_toRenderWindow;
		sf::Event m_renderWindowEvents;
		s2d::Location m_userLocation;
		ImVec4 m_leftButtonColor;
		ImVec4 m_createButtonsColor;
		ImVec4 m_clickedButton;

		std::vector<s2d::ProjectInfo> m_projects; 

		void pollEvents();
		void render();

		void projectData();
		void docsData();

		std::vector<s2d::ProjectInfo> readProjectInfosFromFile();
		void renderProjectDataOrDocs();
		s2d::Location getUserInputForDataToRender();

		std::string getEmptyStringBetween(const std::string& content, const std::string& name, float padding);


	public:
		s2d::ProjectInfo project;

		ProjectSelector();
		~ProjectSelector();

		void update();

	public:
		bool isOpen() { return this->m_ptr_toRenderWindow->isOpen(); }
	};
}


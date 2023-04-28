#pragma once

#include <SFML/Graphics.hpp>
#include <engineComponents/input.h>
#include <physicalComponents/camera.h>
#include <engineComponents/stime.h>
#include <fstream>
#include <stdExtension.h>
#include <data/engineData.h>
#include <manager/initializer.h>
#include <manager/guiRepository.h>

namespace s2d
{
	class UIRealTimeEditorNavigator
	{
	private:
		s2d::GUIRepository* m_ptr_gui_repo;
		s2d::Transform m_cursor;

		float m_arrowSpeed;
		float m_scrollSpeed;

		bool* m_isAnyUIWindowHovered;
		sf::Event* m_ptr_event_engine;
		sf::RenderWindow* m_ptr_renderWindow;

		void navigateRightClick();
		void navigateScrollWheel();
		void navigateArrows();

		void calculateScrollWheelSpeed();

		void setChangedPosition();

	public:
		UIRealTimeEditorNavigator();
		UIRealTimeEditorNavigator(sf::RenderWindow& renderWindow, sf::Event* event, bool* isAnyUIWindowHovered, s2d::GUIRepository& repo);

		void update();
	};
}


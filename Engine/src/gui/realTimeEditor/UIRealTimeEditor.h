#pragma once

#include <SFML/Graphics.hpp>
#include <input.h>
#include <camera.h>
#include <stime.h>
#include <gameObject.h>
#include <fstream>
#include <stdExtension.h>

namespace s2d
{
	class UIRealTimeEditor
	{
	private:
		s2d::Camera m_camera;
		sf::RenderWindow* m_ptr_renderWindow;
		sf::Event* m_ptr_event_engine;

		float m_arrowSpeed;
		float m_scrollSpeed;

		bool* m_isAnyUIWindowHovered;

		void navigateRightClick();
		void navigateScrollWheel();
		void navigateArrows();

		void calculateScrollWheelSpeed();

		void loadCameraSettingsFromFile();
	public: 

		UIRealTimeEditor();
		UIRealTimeEditor(sf::RenderWindow& renderWindow, sf::Event* event, bool* isAnyUIWindowHovered);

		void update();	
	};
}

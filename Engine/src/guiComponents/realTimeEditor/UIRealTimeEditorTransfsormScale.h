#pragma once

#include <SFML/Graphics.hpp>
#include <physicalComponents/gameObject.h>
#include <UIHirachy.h>
#include <guiComponents/UI.h>

namespace s2d
{
	class UIRealTimeEditorTransfsormScale
	{
	private:
		int m_x_vecpos;
		int m_y_vecpos;
		sf::RectangleShape m_y_scaleChanger;
		sf::RectangleShape m_x_scaleChanger;

		s2d::Vector2 m_currentCursorPos;

		bool m_clicked_x;
		bool m_clicked_y;

		s2d::Event* m_event;

		void xScaleChanger(s2d::Sprite* focusedSprite);
		void yScaleChanger(s2d::Sprite* focusedSprite);

		void init();

	public:
		UIRealTimeEditorTransfsormScale();
		UIRealTimeEditorTransfsormScale(s2d::Event* event);

		void update();
	};
}


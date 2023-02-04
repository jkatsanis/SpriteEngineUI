#pragma once

#include "SFML/Graphics.hpp"
#include <UIInspector.h>
#include <guiComponents/UI.h>

namespace s2d
{
	class UIRealTimeEditorTransformPosition
	{
	private:
		s2d::Vector2 m_cursorWorldPos;
		sf::RenderWindow* m_ptr_Window;
		s2d::InspectorState* m_ptr_Inspectorstate;
		s2d::Event* m_windowEvent;
		bool m_realeasedCursorOnSprite;
		bool* m_isAnyUIWindowHovered;

		// Data of sprite we clicked
		s2d::Sprite* m_clickedSprite;
		int m_clickedSpriteId;

		void moveComponent();

		bool checkClick(s2d::Sprite& sprite);
		s2d::Sprite* checkIfMouseClickedOnSprite();

	public:

		UIRealTimeEditorTransformPosition();
		UIRealTimeEditorTransformPosition(sf::RenderWindow* window, s2d::InspectorState* ptr_Inspectorstate, bool* hovered, s2d::Event* windowEvent);

		void update();
	};
}


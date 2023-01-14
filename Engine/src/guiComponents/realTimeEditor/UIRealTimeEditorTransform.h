#pragma once

#include <physicalComponents/sprite.h>
#include <UIInspector.h>
#include <UIHirachy.h>

namespace s2d
{
	// Change values like scale / rotation / position
	class UIRealTimeEditorTransform
	{
	private:
		sf::Vector2f m_cursorWorldPos;
	    sf::RenderWindow* m_ptr_Window;
		s2d::InspectorState* m_ptr_Inspectorstate;
		s2d::Transform m_cursor;
		s2d::Event* m_windowEvent;
		bool m_realeasedCursorOnSprite;
		short m_cursorRangeToClick;
		bool* m_isAnyUIWindowHovered;

		int lowestSort{ -1 };
		int vecpos;

		// Data of sprite we clicked
		s2d::Sprite* m_clickedSprite;
		int m_clickedSpriteId;

		void moveComponent();

		bool checkClick(s2d::Sprite& sprite);
		s2d::Sprite* checkIfMouseClickedOnSprite();
	public:

		UIRealTimeEditorTransform();
		UIRealTimeEditorTransform(sf::RenderWindow* window, s2d::InspectorState* ptr_Inspectorstate, bool* hovered, s2d::Event* windowEvent);

		void update();
	};
}


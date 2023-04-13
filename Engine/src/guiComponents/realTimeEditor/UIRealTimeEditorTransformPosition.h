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
		s2d::InspectorState* m_ptr_Inspectorstate;
		s2d::Event* m_windowEvent;
		bool m_realeasedCursorOnSprite;
		bool* m_isAnyUIWindowHovered;
		s2d::SpriteRepository* m_spriteRepository;

		// Data of sprite we clicked
		s2d::Sprite* m_clickedSprite;
		int m_clickedSpriteId;

		void moveComponent();

		bool checkClick(s2d::Sprite* const sprite);
		s2d::Sprite* checkIfMouseClickedOnSprite();
	public:

		UIRealTimeEditorTransformPosition();
		UIRealTimeEditorTransformPosition(s2d::InspectorState* ptr_Inspectorstate, 
			bool* hovered, s2d::Event* windowEvent, s2d::SpriteRepository& repo);

		void update();
	};
}


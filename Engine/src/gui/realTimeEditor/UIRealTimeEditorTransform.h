#pragma once

#include <sprite.h>
#include <UIInspector.h>

namespace s2d
{
	// Change values like scale / rotation / position
	class UIRealTimeEditorTransform
	{
	private:
	    sf::RenderWindow* m_ptr_Window;
		short m_cursorRangeToClick;
		s2d::InspectorState* m_ptr_Inspectorstate;
			
		bool checkClick(s2d::Sprite& sprite);
		s2d::Sprite* checkIfMouseClickedOnSprite();
	public:

		UIRealTimeEditorTransform();
		UIRealTimeEditorTransform(sf::RenderWindow* window, s2d::InspectorState* ptr_Inspectorstate);

		void update();
	};
}


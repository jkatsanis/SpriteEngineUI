#pragma once

#include <physicalComponents/sprite.h>
#include <UIInspector.h>
#include <UIHirachy.h>
#include <UIRealTimeEditorTransformPosition.h>

namespace s2d
{
	// Change values like scale / rotation / position
	class UIRealTimeEditorTransform
	{
	private:
		UIRealTimeEditorTransformPosition m_positionChanger;

	public:
		UIRealTimeEditorTransform();
		UIRealTimeEditorTransform(sf::RenderWindow* window, s2d::InspectorState* ptr_Inspectorstate, bool* hovered, s2d::Event* windowEvent);

		void update();
	};
}


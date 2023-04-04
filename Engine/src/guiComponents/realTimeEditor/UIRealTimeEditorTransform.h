#pragma once

#include <physicalComponents/sprite.h>
#include <UIInspector.h>
#include <UIHirachy.h>
#include <UIRealTimeEditorTransformPosition.h>
#include <UIRealTimeEditorTransfsormScale.h>
#include <UIToolButtons.h>

namespace s2d
{
	// Change values like scale / rotation / position
	class UIRealTimeEditorTransform
	{
	private:
		s2d::UIRealTimeEditorTransformPosition m_positionChanger;
		s2d::UIRealTimeEditorTransfsormScale m_scaleChanger;

		const s2d::EditorTools* m_currentTool;

	public:
		UIRealTimeEditorTransform();
		UIRealTimeEditorTransform(s2d::InspectorState* ptr_Inspectorstate, bool* hovered,  
			s2d::Event* windowEvent, const s2d::EditorTools* currentToool, s2d::SpriteRepository& repo);

		void update();
	};
}


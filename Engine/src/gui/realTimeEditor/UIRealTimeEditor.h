#pragma once

#include <UIRealTimeEditorNavigator.h>
#include <UIRealTimeEditorTransform.h>
#include <UIInspector.h>

namespace s2d
{
	class UIRealTimeEditor
	{
	private:
		s2d::UIRealTimeEditorNavigator m_navigator;
		s2d::UIRealTimeEditorTransform m_transformChanger;

	public: 

		UIRealTimeEditor();
		UIRealTimeEditor(sf::RenderWindow& renderWindow, sf::Event* event, bool* isAnyUIWindowHovered, s2d::InspectorState* ptr_inspectorState, s2d::Event* windowEvent);

		void update();	
	};
}

#include "UIRealTimeEditor.h"

//Constructor

s2d::UIRealTimeEditor::UIRealTimeEditor() 
{
	this->m_navigator = s2d::UIRealTimeEditorNavigator();
}

s2d::UIRealTimeEditor::UIRealTimeEditor(sf::RenderWindow& window, sf::Event* event, bool* isAnyUIWindowHovered, 
	s2d::InspectorState* ptr_inspectorstate, s2d::Event* windowEvent, s2d::SpriteRepository& repo)
{
	this->m_navigator = s2d::UIRealTimeEditorNavigator(window, event, isAnyUIWindowHovered);
	this->m_transformChanger = s2d::UIRealTimeEditorTransform(ptr_inspectorstate, isAnyUIWindowHovered, windowEvent, 
		repo);
}

//Public functions

void s2d::UIRealTimeEditor::update()
{
	this->m_navigator.update();
	this->m_transformChanger.update();
}
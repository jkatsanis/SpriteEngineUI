#include "UIRealTimeEditor.h"

//Constructor

s2d::UIRealTimeEditor::UIRealTimeEditor() 
{
	this->m_navigator = s2d::UIRealTimeEditorNavigator();
}

s2d::UIRealTimeEditor::UIRealTimeEditor(sf::RenderWindow& window, sf::Event* event, bool* isAnyUIWindowHovered, s2d::InspectorState* ptr_inspectorstate)
{
	this->m_navigator = s2d::UIRealTimeEditorNavigator(window, event, isAnyUIWindowHovered);
	this->m_transformChanger = s2d::UIRealTimeEditorTransform(&window, ptr_inspectorstate);

}

//Public functions

void s2d::UIRealTimeEditor::update()
{
	this->m_navigator.update();
	this->m_transformChanger.update();
}
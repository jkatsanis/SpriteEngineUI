#include "UIRealTimeEditor.h"

//Constructor

s2d::UIRealTimeEditor::UIRealTimeEditor() 
{
	this->m_navigator = s2d::UIRealTimeEditorNavigator();
}

s2d::UIRealTimeEditor::UIRealTimeEditor(sf::RenderWindow& window, sf::Event* event, bool* any_hovered, 
	s2d::InspectorState* ptr_inspectorstate, s2d::Event* windowEvent, s2d::SpriteRepository& repo, s2d::GUIRepository& gui_repo)
{
	this->m_navigator = s2d::UIRealTimeEditorNavigator(window, event, any_hovered, gui_repo);
	this->m_transformChanger = s2d::UIRealTimeEditorTransform(ptr_inspectorstate, any_hovered, windowEvent, 
		repo, gui_repo);
}

//Public functions

void s2d::UIRealTimeEditor::update()
{
	this->m_navigator.update();
	this->m_transformChanger.update();
}
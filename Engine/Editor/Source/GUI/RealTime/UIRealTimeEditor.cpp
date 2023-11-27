#include "UIRealTimeEditor.h"

//Constructor

spe::UIRealTimeEditor::UIRealTimeEditor() 
{
	this->m_navigator = spe::UIRealTimeEditorNavigator();
}

spe::UIRealTimeEditor::UIRealTimeEditor(spe::GameWindow* windowm, spe::SpriteRepository& spriteRepo, spe::GUIRepository& gui_rep)
{
	this->m_navigator.SetRepos(&spriteRepo, &gui_rep);
	this->m_navigator.SetGameWindow(windowm);

	this->m_transformChanger.SetRepos(&spriteRepo, &gui_rep);
	this->m_transformChanger.SetGameWindow(windowm);
}

//Public functions

void spe::UIRealTimeEditor::update()
{
	this->m_navigator.Render();
	this->m_transformChanger.Render();
}
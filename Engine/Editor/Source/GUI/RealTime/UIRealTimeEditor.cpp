#include "UIRealTimeEditor.h"

//Constructor

spe::UIRealTimeEditor::UIRealTimeEditor() 
{
	this->m_Navigator = spe::UIRealTimeEditorNavigator();
}

spe::UIRealTimeEditor::UIRealTimeEditor(spe::GameWindow* windowm, spe::SpriteRepository& spriteRepo, spe::GUIRepository& gui_rep)
{
	this->m_Navigator.SetRepos(&spriteRepo, &gui_rep);
	this->m_Navigator.SetGameWindow(windowm);

	this->m_TransformChanger.SetRepos(&spriteRepo, &gui_rep);
	this->m_TransformChanger.SetGameWindow(windowm);
}

//Public functions

void spe::UIRealTimeEditor::Update()
{
	this->m_Navigator.Render();
	this->m_TransformChanger.Render();
}
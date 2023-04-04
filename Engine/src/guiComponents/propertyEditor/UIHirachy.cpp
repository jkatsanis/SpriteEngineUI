
#include "UIHirachy.h"

//Constructor

s2d::UIHirachy::UIHirachy()
{
	this->isHovered = false;
	this->m_menuDisplayer = s2d::UIHirachyMenuDisplayer();
}

s2d::UIHirachy::UIHirachy(s2d::SpriteRepository& repo)
{
	this->isHovered = false;
	this->m_menuDisplayer = s2d::UIHirachyMenuDisplayer(&this->m_spriteDisplayer, repo);
	this->m_spriteDisplayer = s2d::UIHirachySpriteDisplayer(repo);
}									

//Public functions

void s2d::UIHirachy::createHirachyWindow()
{
	//Pushing transperany
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.1f);

	if (ImGui::Begin("Hirachy", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
	{
		this->displayHirachyComponents();
		this->isHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) || this->m_menuDisplayer.isPopUpOpen;
		ImGui::End();
	}

	ImGui::PopStyleVar();

	this->m_spriteDisplayer.displayWindowWhenChildIsGettingDragged();
}

//private functions

void s2d::UIHirachy::displayHirachyComponents()
{
	//Setting HY window size
	ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2(250.0f, 1080.0f));

	//Creating additonal windows
	this->m_menuDisplayer.createMenuPopup();

	//Adding sprites to hirachy
	this->m_spriteDisplayer.addSpritesToHirachy();
}


s2d::Sprite* s2d::UIHirachy::s_selectedSprite = nullptr;


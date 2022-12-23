#include "UIToolButtons.h"

s2d::UIToolButtons::UIToolButtons()
{
	this->isHovered = false;
}

void s2d::UIToolButtons::createToolsAndButtons()
{
	//Pushing transperany
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.1f);

	ImGui::Begin(" ", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
	ImGui::SetWindowPos(ImVec2(250, 0));
	ImGui::SetWindowSize(ImVec2(1280, 100.0f));

	//Button at the top to click and play game 
	askWithButtonForPlayGame();

	this->isHovered = ImGui::IsWindowHovered();

	ImGui::End();

	ImGui::PopStyleVar();
}

void s2d::UIToolButtons::setBackgroundColorToSave(s2d::Vector3 color)
{
	this->m_windowbBackgroundToSave = color;
}

void s2d::UIToolButtons::askWithButtonForPlayGame()
{
	ImGui::SetCursorPosX(500);
	ImGui::SetCursorPos(ImVec2(960 - 320 - 10, ImGui::GetCursorPosY()));

	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_PLAY))
	{
		//EXETUING NEW EXE
		s2d::Animator::stopAllAnimations();

		//SAVING THE FILE AGAIN SINCE WE PRESS "PLAY"
		s2d::flc::createSaveFile(s2d::Sprite::activeSprites);
		s2d::flc::createWindowBackgroundSaveFile(this->m_windowbBackgroundToSave);
		s2d::flc::createCameraSaveFile(*s2d::GameObject::ptr_camera_tRealTimeEditor);
		s2d::flc::createIndexSaveFile();

		system("..\\Assets\\x64\\Debug\\Assets.exe");
	}

}

#include "UIToolButtons.h"

s2d::UIToolButtons::UIToolButtons()
{
	this->editorTools = s2d::EditorTools::PositionTool;
	this->isHovered = false;

	this->m_tools[0] = s2d::Tool(s2d::EditorTools::PositionTool, ICON_FA_ARROWS);
	this->m_tools[1] = s2d::Tool(s2d::EditorTools::ScaleTool, ICON_FA_PLUS);

	this->m_tools[0].background = true;
	this->m_clickedOnBtn = true;
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
	this->askWithButtonForPlayGame();
	this->toolSelector();

	this->isHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

	ImGui::End();

	ImGui::PopStyleVar();
}

void s2d::UIToolButtons::setBackgroundColorToSave(const s2d::Vector3& color)
{
	this->m_windowbBackgroundToSave = color;
}

void s2d::UIToolButtons::askWithButtonForPlayGame()
{
	ImGui::SetCursorPosX(500);
	ImGui::SetCursorPos(ImVec2(960 - 320 - 10, ImGui::GetCursorPosY()));

	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_PLAY) || s2d::Input::onKeyRelease(s2d::KeyBoardCode::F5))
	{
		//EXETUING NEW EXE
		s2d::Animator::stopAllAnimations();

		//SAVING THE FILE AGAIN SINCE WE PRESS "PLAY"
		s2d::flc::createSaveFile(s2d::Sprite::activeSprites);
		s2d::flc::createWindowBackgroundSaveFile(this->m_windowbBackgroundToSave);
		s2d::flc::createCameraSaveFile(*s2d::GameObject::ptr_camera_tRealTimeEditor);
		s2d::flc::createIndexSaveFile();
	
		wchar_t engineDirectory[MAX_PATH];
		if (!GetCurrentDirectory(MAX_PATH, engineDirectory))
		{
			std::cout << "FAILED GETTING DIRECTORY";
		}

		// ! Setting the current directory to the of the project.( the relative dir)

		// Calculate the length of the wide string, including the null terminator.
		int wideStringLength = MultiByteToWideChar(CP_UTF8, 0, s2d::EngineData::s_pathToUserProject.c_str(), -1, NULL, 0);

		// Allocate a buffer for the wide string.
		wchar_t* wideString = new wchar_t[wideStringLength];

		// Convert the narrow string to a wide string.
		MultiByteToWideChar(CP_UTF8, 0, s2d::EngineData::s_pathToUserProject.c_str(), -1, wideString, wideStringLength);

		if (!SetCurrentDirectory(wideString))
		{
			std::cout << "FAILED SETTING DIR";	
		}

		std::string path = "x64\\Debug\\Assets.exe";

		//Starting the game
		system(path.c_str());

		//Reseting the dir
		if (!SetCurrentDirectory(engineDirectory))
		{
			std::cout << "FAILED SETTING DIR";
		}

		delete[] wideString;
	}

}

void s2d::UIToolButtons::toolSelector()
{
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 35);

	for (int i = 0; i < TOOLS_SIZE; i++)
	{
		if (this->m_tools[i].background)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, REAL_EDITOR_BUTTON_BG_COLOR);
		}
		if (s2d::FontManager::displaySmybolAsButton(this->m_tools[i].icon.c_str()))
		{
			this->m_clickedOnBtn = true;
			this->editorTools = this->m_tools[i].tool;
		}
		if (this->m_tools[i].background)
		{
			ImGui::PopStyleColor();
		}
		if (this->m_clickedOnBtn)
		{
			this->removeBackgroundFromButtons();
			this->m_clickedOnBtn = false;
			this->m_tools[i].background = true;
		}
		ImGui::SameLine();
	}
}

void s2d::UIToolButtons::removeBackgroundFromButtons()
{
	for (int i = 0; i < TOOLS_SIZE; i++)
	{
		this->m_tools[i].background = false;
	}
}



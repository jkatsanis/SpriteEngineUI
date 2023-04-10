#include "UIToolButtons.h"

// Constructor

s2d::UIToolButtons::UIToolButtons()
{
	this->init();
}

s2d::UIToolButtons::UIToolButtons(s2d::SpriteRepository& spriteRepo)
{
	this->init();
	this->m_spriteRepository = &spriteRepo;
}

void s2d::UIToolButtons::init()
{
	this->m_editor_tools = s2d::EditorTools::PositionTool;
	this->isHovered = false;

	this->m_tools[0] = s2d::Tool(s2d::EditorTools::PositionTool, ICON_FA_ARROWS);
	this->m_tools[1] = s2d::Tool(s2d::EditorTools::ScaleTool, ICON_FA_PLUS);

	this->m_tools[0].background = true;
	this->m_clickedOnBtn = true;
}

// Public functions

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
	this->buildProjectIntoFolder();
	this->hotkeys();

	this->isHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

	ImGui::End();

	ImGui::PopStyleVar();
}

void s2d::UIToolButtons::setBackgroundColorToSave(const s2d::Vector3& color)
{
	this->m_windowbBackgroundToSave = color;
}

// Private functions

void s2d::UIToolButtons::buildProjectIntoFolder()
{
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 1100);
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Save", "CTRL + S"))
		{
			s2d::flc::saveEverything(this->m_windowbBackgroundToSave, *this->m_spriteRepository);
		}	
		if (ImGui::MenuItem("Build", "CTRL + B"))
		{
			this->build();
		}
		ImGui::EndMenu();
	}

}

void s2d::UIToolButtons::hotkeys()
{
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::LControl)
		&& s2d::Input::onKeyPress(s2d::KeyBoardCode::S))
	{
		s2d::flc::saveEverything(this->m_windowbBackgroundToSave, *this->m_spriteRepository);
	}

	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::LControl)
		&& s2d::Input::onKeyPress(s2d::KeyBoardCode::B))
	{
		this->build();
	}
}

void s2d::UIToolButtons::build()
{
	s2d::flc::saveEverything(this->m_windowbBackgroundToSave, *this->m_spriteRepository);
	const std::string PATH = s2d::EngineData::s_pathToUserProject + "\\" + s2d::EngineData::s_nameOfUserProject;
	std::filesystem::path TARGET_PATH = s2d::EngineData::s_pathToUserProject + "\\" + s2d::EngineData::s_nameOfUserProject;
	std::filesystem::path FILES_IN_FOLDER[FILE_AMOUNT] =
	{
		PATH_TO_USER_DEBUG_FOLDER"Assets.exe",
		PATH_TO_USER_DEBUG_FOLDER"sfml-audio-d-2.dll",
		PATH_TO_USER_DEBUG_FOLDER"sfml-graphics-d-2.dll",
		PATH_TO_USER_DEBUG_FOLDER"sfml-network-d-2.dll",
		PATH_TO_USER_DEBUG_FOLDER"sfml-system-d-2.dll",
		PATH_TO_USER_DEBUG_FOLDER"sfml-window-d-2.dll",
	};

	s2d::flc::removeDir(PATH);
	std::filesystem::create_directories(TARGET_PATH);

	for (int i = 0; i < FILE_AMOUNT; i++)
	{
		auto fileTarget = TARGET_PATH / FILES_IN_FOLDER[i].filename();

		if (std::filesystem::is_regular_file(FILES_IN_FOLDER[i]))
		{
			std::filesystem::copy_file(FILES_IN_FOLDER[i], fileTarget, std::filesystem::copy_options::overwrite_existing);
		}
	}

	s2d::flc::copyDir(s2d::EngineData::s_pathToUserProject + "\\assets", PATH, "\\assets", { "\\src\\", ".cpp", ".h" } );
	s2d::flc::copyDir(s2d::EngineData::s_pathToUserProject + "\\engine", PATH, "\\engine", { "\\src\\", ".cpp", ".h" } );
}

void s2d::UIToolButtons::askWithButtonForPlayGame()
{
	ImGui::SetCursorPosX(500);
	ImGui::SetCursorPos(ImVec2(960 - 320 - 10, ImGui::GetCursorPosY()));

	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_PLAY) || s2d::Input::onKeyRelease(s2d::KeyBoardCode::F5))
	{
		s2d::flc::saveEverything(this->m_windowbBackgroundToSave, *this->m_spriteRepository);

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
			this->m_editor_tools = this->m_tools[i].tool;
			this->m_spriteRepository->current_tool = this->m_editor_tools;
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



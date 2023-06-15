 #include "UIToolButtons.h"

// Constructor

s2d::UIToolButtons::UIToolButtons()
{
	this->init();
}

s2d::UIToolButtons::UIToolButtons(s2d::SpriteRepository& spriteRepo)
{
	this->init();
	this->m_ptr_repo = &spriteRepo;
}

void s2d::UIToolButtons::init()
{
	this->m_editor_tools = s2d::EditorTools::PositionTool;
	this->is_hovered = false;

	this->m_tools[0] = s2d::Tool(s2d::EditorTools::PositionTool, ICON_FA_ARROWS, "Position");
	this->m_tools[1] = s2d::Tool(s2d::EditorTools::ScaleTool, ICON_FA_PLUS, "Scaling");

	this->m_tools[0].background = true;
	this->m_clicked_on_btn = true;
}

// Public functions

void s2d::UIToolButtons::createToolsAndButtons()
{
	this->hotkeys();
	this->renderMainMenuBar();

	ImGui::Begin("##tools-buttons", NULL, DEFAULT_WINDOW_FLAGS | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);

	// Render
	ImGui::SetCursorPos(ImVec2(0, 0));
	this->playGameButton();
	this->toolSelector();
	ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size + 0.2f);
	ImGui::SetWindowPos(ImVec2(1920 - this->m_ptr_gui_repo->ptr_inspector_window_size->x - 120, 56));
	ImGui::SetWindowSize(ImVec2(120, 30));
	ImGui::End();
}
void s2d::UIToolButtons::setBackgroundColorToSave(const s2d::Vector3& color)
{
	this->m_window_background_to_save = color;
}

// Private functions

void s2d::UIToolButtons::renderMainMenuBar()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1019f, .1019f, .1019f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, WINDOW_SIZE_Y_TOOL_BUTTONS));
	if (ImGui::BeginMainMenuBar())
	{
		this->buildProjectIntoFolder();
		this->renderWindowSelecter();
		this->renderToolSelector();

		ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);
		ImGui::EndMainMenuBar();
	}	
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void s2d::UIToolButtons::renderToolSelector()
{
	ImGui::SetCursorPosY(3);
	if (ImGui::BeginMenu("Tools"))
	{
		for (int i = 0; i < TOOLS_AMOUNT; i++)
		{
			if (ImGui::MenuItem(this->m_tools[i].tool_name.c_str()))
			{
				this->m_editor_tools = this->m_tools[i].tool;
				this->m_ptr_repo->current_tool = this->m_editor_tools;
				this->removeBackgroundFromButtons();
				this->m_tools[i].background = true;
			}
		}
		ImGui::EndMenu();
	}
}

void s2d::UIToolButtons::buildProjectIntoFolder()
{
	ImGui::SetCursorPosY(3);
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Save", "CTRL + S"))
		{
			s2d::flc::cleanUp(*this->m_ptr_repo, true);
			s2d::flc::saveEverything(this->m_window_background_to_save, *this->m_ptr_repo, *this->m_ptr_gui_repo);
		}	
		if (ImGui::MenuItem("Build", "CTRL + B"))
		{
			this->build();
		}

		ImGui::EndMenu();
	}
}

void s2d::UIToolButtons::renderWindowSelecter()
{
	// Hard coded windows
	ImGui::SetCursorPosY(3);
	if (ImGui::BeginMenu("Window"))
	{
		if (ImGui::MenuItem("Hierarchy"))
		{
			s2d::UIInfo::s_is_hierarchy_open.setOpen();
		}
		if (ImGui::MenuItem("Inspector"))
		{
		}
		if (ImGui::MenuItem("Content Browser"))
		{
			s2d::UIInfo::s_is_asset_folder_open.setOpen();
		}
		ImGui::EndMenu();
	}
}

void s2d::UIToolButtons::hotkeys()
{
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::LControl)
		&& s2d::Input::onKeyPress(s2d::KeyBoardCode::S))
	{

		s2d::flc::cleanUp(*this->m_ptr_repo, true);
		s2d::flc::saveEverything(this->m_window_background_to_save, *this->m_ptr_repo, *this->m_ptr_gui_repo);
	}

	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::LControl)
		&& s2d::Input::onKeyPress(s2d::KeyBoardCode::B))
	{
		this->build();
	}
}

void s2d::UIToolButtons::build()
{
	s2d::flc::saveEverything(this->m_window_background_to_save, *this->m_ptr_repo, *this->m_ptr_gui_repo);
	const std::string PATH = s2d::EngineData::s_path_to_user_project + "\\" + s2d::EngineData::s_nameOfUserProject;
	const std::filesystem::path TARGET_PATH = s2d::EngineData::s_path_to_user_project + "\\" + s2d::EngineData::s_nameOfUserProject;
	const std::filesystem::path FILES_IN_FOLDER[FILE_AMOUNT] =
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
		const auto fileTarget = TARGET_PATH / FILES_IN_FOLDER[i].filename();

		if (std::filesystem::is_regular_file(FILES_IN_FOLDER[i]))
		{
			std::filesystem::copy_file(FILES_IN_FOLDER[i], fileTarget, std::filesystem::copy_options::overwrite_existing);
		}
	}

	s2d::flc::copyDir(s2d::EngineData::s_path_to_user_project + "\\assets", PATH, "\\assets", { "\\src\\", ".cpp", ".h" } );
	s2d::flc::copyDir(s2d::EngineData::s_path_to_user_project + "\\engine", PATH, "\\engine", { "\\src\\", ".cpp", ".h" } );
}

void s2d::UIToolButtons::playGameButton()
{
	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_PLAY) || s2d::Input::onKeyRelease(s2d::KeyBoardCode::F5))
	{
		s2d::flc::saveEverything(this->m_window_background_to_save, *this->m_ptr_repo, *this->m_ptr_gui_repo);

		wchar_t engineDirectory[MAX_PATH];
		if (!GetCurrentDirectory(MAX_PATH, engineDirectory))
		{
			std::cout << "FAILED GETTING DIRECTORY";
		}

		// ! Setting the current directory to the of the project.( the relative dir)

		// Calculate the length of the wide string, including the null terminator.
		int wideStringLength = MultiByteToWideChar(CP_UTF8, 0, s2d::EngineData::s_path_to_user_project.c_str(), -1, NULL, 0);

		// Allocate a buffer for the wide string.
		wchar_t* wideString = new wchar_t[wideStringLength];

		// Convert the narrow string to a wide string.
		MultiByteToWideChar(CP_UTF8, 0, s2d::EngineData::s_path_to_user_project.c_str(), -1, wideString, wideStringLength);

		if (!SetCurrentDirectory(wideString))
		{
			std::cout << "FAILED SETTING DIR";	
		}

		// if you get "the command has not been found msg" you have not built the project!
		const std::string path = "x64\\Debug\\Assets.exe";

		//Starting the game
		system(path.c_str());
		std::cout << "LOG [WARNING] If the game doesnt start make sure that you have built the program into a .exe in the Assets project!!!" << std::endl;

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
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 28);
	for (int i = 0; i < TOOLS_AMOUNT; i++)
	{
		if (this->m_tools[i].background)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, REAL_EDITOR_BUTTON_BG_COLOR);
		}
		ImGui::SetCursorPosY(0);
		if (s2d::FontManager::displaySmybolAsButton(this->m_tools[i].icon.c_str()))
		{
			this->m_clicked_on_btn = true;
			this->m_editor_tools = this->m_tools[i].tool;
			this->m_ptr_repo->current_tool = this->m_editor_tools;
		}
		if (this->m_tools[i].background)
		{
			ImGui::PopStyleColor();
		}
		if (this->m_clicked_on_btn)
		{
			this->removeBackgroundFromButtons();
			this->m_clicked_on_btn = false;
			this->m_tools[i].background = true;
		}
		ImGui::SameLine();
	}
}

void s2d::UIToolButtons::removeBackgroundFromButtons()
{
	for (int i = 0; i < TOOLS_AMOUNT; i++)
	{
		this->m_tools[i].background = false;
	}
}



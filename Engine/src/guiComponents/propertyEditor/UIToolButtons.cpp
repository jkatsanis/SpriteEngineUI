#include "UIToolButtons.h"

// Constructor

s2d::UIToolButtons::UIToolButtons()
{
	this->init();
}

s2d::UIToolButtons::UIToolButtons(s2d::SpriteRepository& spriteRepo, std::vector<std::string>& scene_names)
{
	this->init();
	this->m_ptr_repo = &spriteRepo;
	this->m_ptr_scene_names = &scene_names;
}

s2d::UIToolButtons::~UIToolButtons()
{
	this->m_new_scene_name[0] = '\0';
}

void s2d::UIToolButtons::init()
{
	this->m_switch_scene_name = "";
	this->m_new_scene_name[0] = '\0';
	this->m_add_scene_mode = false;
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
	this->is_hovered = false;

	this->hotkeys();
	this->renderMainMenuBar();

	ImGui::Begin("##tools-buttons", NULL, DEFAULT_WINDOW_FLAGS | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
	// Render
	ImGui::SetCursorPos(ImVec2(0, 0));
	this->playGameButton();
	this->toolSelector();

	if (ImGui::IsMouseReleased(0))
	{
		if (this->m_update_event_to_set)
		{
			this->m_update_event_to_set = false;
			s2d::Event::s_handle_other_events = true;
		}
	}

	if (!this->is_hovered)
	{
		this->is_hovered = s2d::UI::isHovered(ImVec2(0, 0), ImVec2(1920, 50));
	}
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
		this->renderSceneSelector();

		ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);
		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void s2d::UIToolButtons::renderToolSelector()
{
	if (ImGui::BeginMenu("Tools"))
	{
		this->is_hovered = true;

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

void s2d::UIToolButtons::renderSceneSelector()
{
	if (ImGui::BeginMenu("Scenes"))
	{
		this->is_hovered = true;

		for (size_t i = 0; i < this->m_ptr_scene_names->size(); i++)
		{
			const std::string name = this->m_ptr_scene_names->at(i);
			if (name == s2d::EngineData::s_scene)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, SPRITE_SELECTED_COLOR);
				ImGui::Text(name.c_str());
				ImGui::PopStyleColor();
			}
			else
			{
				ImGui::Text(name.c_str());
			}
			ImVec2 cursor = ImVec2(ImGui::GetCursorPosX() + 115, ImGui::GetCursorPosY() - 30);
			std::string identy = "##" + name;
			if (s2d::FontManager::displaySymbolAsButtonWithWidthAndCursorPos(ICON_FA_TRASH, cursor, ImVec2(30, 30), identy))
			{
				this->removeScene(name);
			}
			cursor.x += 27;
			cursor.y += 1.5f;
			identy += "symbol";
			if (s2d::FontManager::displaySymbolAsButtonWithWidthAndCursorPos(ICON_FA_EDIT, cursor, ImVec2(30, 30), identy))
			{
				this->m_switch_scene_name = name;
			}
			ImGui::Dummy(ImVec2(0, 5));
		}
		ImGui::Separator();
		if (ImGui::MenuItem("Add scene"))
		{
			this->m_add_scene_mode = true;
		}

		ImGui::EndMenu();
	}
	const std::string scene_text = "Currently editing: " + s2d::EngineData::s_scene;
	ImGui::SetCursorPosX(1880 - ImGui::CalcTextSize(scene_text.c_str()).x);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
	ImGui::Text(scene_text.c_str());
	ImGui::PopStyleColor();

	this->switchScene(this->m_switch_scene_name);
	this->renderSceneAddPopup();
}

void s2d::UIToolButtons::removeScene(const std::string& scene)
{
	if (this->m_ptr_scene_names->size() <= 1)
	{
		return;
	}
	size_t idx = -1;
	for (size_t i = 0; i < this->m_ptr_scene_names->size(); i++)
	{
		if (this->m_ptr_scene_names->at(i) == scene)
		{
			idx = i;
			break;
		}
	}
	if (idx != -1)
	{
		std::removeAtVec(*this->m_ptr_scene_names, (int)idx);
		const std::string path = PATH_TO_USER_SAVES_FOLDER + "\\" + scene;
		std::filesystem::remove_all(path);
	}

	if (s2d::EngineData::s_scene == scene)
	{
		s2d::EngineData::s_scene = this->m_ptr_scene_names->at(0);
	}
}

void s2d::UIToolButtons::renderSceneAddPopup()
{
	if (!this->m_add_scene_mode)
	{
		return;
	}

	const static ImVec2 window_size = ImVec2(200, 75);

	if (ImGui::Begin("##create-scene", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
	{
		ImGui::Text("Add scene");
		ImGui::Separator();

		ImGui::SetNextItemWidth(150);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 7));
		ImGui::InputTextWithHint("##add-scene", "<name>", this->m_new_scene_name, CHAR_MAX);

		ImGui::PopStyleVar();

		s2d::UI::sameLine(0);

		if (s2d::FontManager::displaySmybolAsButton(ICON_FA_ARROW_RIGHT))
		{
			this->createScene();
		}

		ImGui::SetWindowSize(window_size);
		ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);
		if (!this->is_hovered)
		{
			this->is_hovered = s2d::UI::isHovered(window_size, ImGui::GetWindowPos());
		}
		ImGui::End();
	}
	if (ImGui::IsKeyReleased(ImGuiKey_Escape))
	{
		this->m_add_scene_mode = false;
		this->m_new_scene_name[0] = '\0';
	}

	if (this->m_add_scene_mode && ImGui::IsKeyReleased(ImGuiKey_Enter))
	{
		this->createScene();
	}
}

void s2d::UIToolButtons::switchScene(const std::string& scene)
{
	if (this->m_switch_scene_name == "")
	{
		return;
	}
	ImGui::SetNextWindowFocus();
	if (ImGui::Begin("close-scene-popup", NULL,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 7.5f);
		const ImVec2 CURSOR_POS = ImGui::GetCursorPos();
		if (ImGui::Button("Save"))
		{
			s2d::flc::cleanUp(*this->m_ptr_repo, true);
			s2d::flc::saveEverything(this->m_window_background_to_save, *this->m_ptr_repo, *this->m_ptr_gui_repo, *this->m_ptr_scene_names);
			s2d::EngineData::s_scene = scene;
			this->m_switch_scene_name = "";
		}
		ImGui::SameLine();
		if (ImGui::Button("Don't save"))
		{
			s2d::EngineData::s_scene = scene;
			this->m_switch_scene_name = "";
		}
		if (ImGui::IsKeyReleased(ImGuiKey_Escape))
		{
			this->m_switch_scene_name = "";
		}

		s2d::UI::setWindowScreenMiddle(ImVec2(200, 50));
		ImGui::SetWindowSize(ImVec2(200, 50)),
			ImGui::End();
	}
}

void s2d::UIToolButtons::buildProjectIntoFolder()
{
	if (ImGui::BeginMenu("File"))
	{
		this->is_hovered = true;
		if (ImGui::MenuItem("Save", "CTRL + S"))
		{
			s2d::flc::cleanUp(*this->m_ptr_repo, true);
			s2d::flc::saveEverything(this->m_window_background_to_save, *this->m_ptr_repo, *this->m_ptr_gui_repo, *this->m_ptr_scene_names);
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
	if (ImGui::BeginMenu("Window"))
	{
		this->is_hovered = true;
		if (ImGui::MenuItem("Hierarchy"))
		{
			s2d::UIInfo::s_is_hierarchy_open.setOpen();
		}
		if (ImGui::MenuItem("Inspector"))
		{
			s2d::UIInfo::s_is_inspector_open.setOpen();
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
		s2d::flc::saveEverything(this->m_window_background_to_save, *this->m_ptr_repo, *this->m_ptr_gui_repo, *this->m_ptr_scene_names);
	}

	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::LControl)
		&& s2d::Input::onKeyPress(s2d::KeyBoardCode::B))
	{
		this->build();
	}
}

void s2d::UIToolButtons::build()
{
	s2d::flc::cleanUp(*this->m_ptr_repo, true);
	s2d::flc::saveEverything(this->m_window_background_to_save, *this->m_ptr_repo, *this->m_ptr_gui_repo, *this->m_ptr_scene_names);
	const std::string PATH = s2d::EngineData::s_path_to_user_project + "\\" + s2d::EngineData::s_name_of_user_project;
	const std::filesystem::path TARGET_PATH = s2d::EngineData::s_path_to_user_project + "\\" + s2d::EngineData::s_name_of_user_project;
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

	s2d::flc::copyDir(s2d::EngineData::s_path_to_user_project + "\\assets", PATH, "\\assets", { "\\src\\", ".cpp", ".h" });
	s2d::flc::copyDir(s2d::EngineData::s_path_to_user_project + "\\engine", PATH, "\\engine", { "\\src\\", ".cpp", ".h" });
}

void s2d::UIToolButtons::createScene()
{
	this->m_add_scene_mode = false;
	const std::string scene_name = std::string(this->m_new_scene_name);
	if (scene_name == "")
	{
		return;
	}
	if (!std::isEqualWithAny(scene_name, *this->m_ptr_scene_names))
	{
		// ADDING THE SCENE			
		const std::string input_dir = PATH_TO_USER_FIRST_SCENE;
		const std::string output_dir = s2d::EngineData::s_path_to_user_project + "\\engine\\saves\\";
		s2d::flc::copyDir(input_dir, output_dir, scene_name);
		this->m_ptr_scene_names->push_back(scene_name);

		s2d::flc::createSceneSaveFile(*this->m_ptr_scene_names);
	}
	this->m_new_scene_name[0] = '\0';
}

void s2d::UIToolButtons::playGameButton()
{
	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_PLAY) || s2d::Input::onKeyRelease(s2d::KeyBoardCode::F5))
	{
		s2d::flc::cleanUp(*this->m_ptr_repo, true);
		s2d::flc::saveEverything(this->m_window_background_to_save, *this->m_ptr_repo, *this->m_ptr_gui_repo, *this->m_ptr_scene_names);

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
		if (s2d::FontManager::displaySmybolAsButton(this->m_tools[i].icon.c_str())) {}
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			this->m_clicked_on_btn = true;
			this->m_editor_tools = this->m_tools[i].tool;
			this->m_ptr_repo->current_tool = this->m_editor_tools;
			this->m_update_event_to_set = true;

			s2d::Event::s_handle_other_events = false;
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
#include "UITopbar.h"

// Constructor



void spe::UITopbar::Init()
{
	this->m_display_engine_info = true;
	this->m_switch_scene_name = "";
	this->m_new_scene_name[0] = '\0';
	this->m_add_scene_mode = false;
	this->m_editor_tools = spe::EditorTools::PositionTool;
	this->Hovered = false;

	this->m_tools[0] = spe::Tool(spe::EditorTools::PositionTool, ICON_FA_ARROWS, "Position");
	this->m_tools[1] = spe::Tool(spe::EditorTools::ScaleTool, ICON_FA_PLUS, "Scaling");

	this->m_tools[0].background = true;
	this->m_clicked_on_btn = true;
}

// Public functions

void spe::UITopbar::Render()
{
	this->Hovered = false;

	this->hotkeys();
	this->renderMainMenuBar();

	ImGui::Begin("##tools-buttons", NULL, DEFAULT_FLAGS);
	// Render
	ImGui::SetCursorPos(ImVec2(0, 0));
	this->playGameButton();
	this->toolSelector();

	if (ImGui::IsMouseReleased(0))
	{
		if (this->m_update_event_to_set)
		{
			this->m_update_event_to_set = false;
		}
	}

	if (!this->Hovered)
	{
		this->Hovered = spe::UIUtility::IsHovered(ImVec2(0, 0), ImVec2(1920, 50));
	}
	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
	ImGui::SetWindowPos(ImVec2(1920 - 200 - 120, 56)); // Todo
	ImGui::SetWindowSize(ImVec2(120, 30));
	ImGui::End();
}

// Private functions

void spe::UITopbar::renderMainMenuBar()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1019f, .1019f, .1019f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, WINDOW_SIZE_Y_TOOL_BUTTONS));

	if (ImGui::BeginMainMenuBar())
	{
		this->buildProjectIntoFolder();
		this->renderWindowSelecter();
		this->renderToolSelector();
		this->renderSceneSelector();
		this->displayEngineInfo();

		ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void spe::UITopbar::renderToolSelector()
{
	if (ImGui::BeginMenu("Tools"))
	{
		this->Hovered = true;

		for (int i = 0; i < TOOLS_AMOUNT; i++)
		{
			if (ImGui::MenuItem(this->m_tools[i].tool_name.c_str()))
			{
				this->m_editor_tools = this->m_tools[i].tool;
				this->m_ptr_GUIRepo->Tools = this->m_editor_tools;
				this->removeBackgroundFromButtons();
				this->m_tools[i].background = true;
			}
		}
		ImGui::EndMenu();
	}
}

void spe::UITopbar::renderSceneSelector()
{
	if (ImGui::BeginMenu("Scenes"))
	{
		this->Hovered = true;

		for (size_t i = 0; i < this->m_ptr_SceneHandler->TotalScenes.size(); i++)
		{
			const std::string name = this->m_ptr_SceneHandler->TotalScenes.at(i);
			if (name == this->m_ptr_SceneHandler->CurrentScene)
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
			if (spe::Style::DisplaySymbolAsButtonWithWidthAndCursorPos(ICON_FA_TRASH, cursor, ImVec2(30, 30), identy))
			{
				this->removeScene(name);
			}
			cursor.x += 27;
			cursor.y += 1.5f;
			identy += "symbol";
			if (spe::Style::DisplaySymbolAsButtonWithWidthAndCursorPos(ICON_FA_EDIT, cursor, ImVec2(30, 30), identy))
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

	this->switchScene(this->m_switch_scene_name);
	this->renderSceneAddPopup();
}

void spe::UITopbar::removeScene(const std::string& scene)
{
	if (this->m_ptr_SceneHandler->TotalScenes.size() <= 1)
	{
		return;
	}
	size_t idx = -1;
	for (size_t i = 0; i < this->m_ptr_SceneHandler->TotalScenes.size(); i++)
	{
		if (this->m_ptr_SceneHandler->TotalScenes.at(i) == scene)
		{
			idx = i;
			break;
		}
	}
	if (idx != -1)
	{
		this->m_ptr_SceneHandler->DeleteScene(scene);
	}

	if (this->m_ptr_SceneHandler->CurrentScene == scene)
	{
		this->m_ptr_SceneHandler->CurrentScene = this->m_ptr_SceneHandler->TotalScenes.at(0);
	}
}

void spe::UITopbar::renderSceneAddPopup()
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

		spe::UIUtility::SameLine;

		if (spe::Style::DisplaySmybolAsButton(ICON_FA_ARROW_RIGHT))
		{
			this->createScene();
		}

		ImGui::SetWindowSize(window_size);
		ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
		if (!this->Hovered)
		{
			this->Hovered =  spe::UIUtility::IsHovered(window_size, ImGui::GetWindowPos());
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

void spe::UITopbar::switchScene(const std::string& scene)
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
			spe::Savesystem::SaveEverything(*this->m_ptr_Repo, *this->m_ptr_GUIRepo, *this->m_ptr_SceneHandler);
			
			this->m_ptr_SceneHandler->CurrentScene = scene;
			this->m_switch_scene_name = "";
		}
		ImGui::SameLine();
		if (ImGui::Button("Don't save"))
		{
			this->m_ptr_SceneHandler->CurrentScene = scene;	
			this->m_switch_scene_name = "";
		}
		if (ImGui::IsKeyReleased(ImGuiKey_Escape))
		{
			this->m_switch_scene_name = "";
		}

		spe::UIUtility::SetWindowScreenMiddle(ImVec2(200, 50));
		ImGui::SetWindowSize(ImVec2(200, 50)),
			ImGui::End();
	}
}

void spe::UITopbar::displayEngineInfo()
{
	if (!this->m_display_engine_info)
	{
		return;
	}

	const std::string scene_text = "Editing: " + this->m_ptr_SceneHandler->CurrentScene;
	ImGui::SetCursorPosX(1880 - ImGui::CalcTextSize(scene_text.c_str()).x);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
	ImGui::Text(scene_text.c_str());

	const std::string info_text = "FPS: " + std::to_string(spe::Time::s_fps);
	ImGui::SetCursorPosX(1650 - ImGui::CalcTextSize(info_text.c_str()).x);
	ImGui::Text(info_text.c_str());

	ImGui::PopStyleColor();
}

void spe::UITopbar::buildProjectIntoFolder()
{
	if (ImGui::BeginMenu("File"))
	{
		this->Hovered = true;
		if (ImGui::MenuItem("Save", "CTRL + S"))
		{
			spe::Savesystem::SaveEverything(*this->m_ptr_Repo, *this->m_ptr_GUIRepo, *this->m_ptr_SceneHandler);
		}
		if (ImGui::MenuItem("Build", "CTRL + B"))
		{
			this->build();
		}

		ImGui::EndMenu();
	}
}

void spe::UITopbar::renderWindowSelecter()
{
	// Hard coded windows
	if (ImGui::BeginMenu("Window"))
	{
		this->Hovered = true;
		if (ImGui::MenuItem("Hierarchy"))
		{
			this->m_ptr_GUIRepo->HierarchyData.SetOpen();
		}
		if (ImGui::MenuItem("Inspector"))
		{
			this->m_ptr_GUIRepo->InspectorData.SetOpen();
		}
		if (ImGui::MenuItem("Content Browser"))
		{
			this->m_ptr_GUIRepo->AssetFolderData.SetOpen();
		}
		if (ImGui::MenuItem("Engine info"))
		{
			this->m_display_engine_info = !this->m_display_engine_info;
		}
		ImGui::EndMenu();
	}
}

void spe::UITopbar::hotkeys()
{
	if (spe::Input::onKeyHold(spe::KeyBoardCode::LControl)
		&& spe::Input::onKeyPress(spe::KeyBoardCode::S))
	{
		spe::Savesystem::SaveEverything(*this->m_ptr_Repo, *this->m_ptr_GUIRepo, *this->m_ptr_SceneHandler);
	}

	if (spe::Input::onKeyHold(spe::KeyBoardCode::LControl)
		&& spe::Input::onKeyPress(spe::KeyBoardCode::B))
	{
		this->build();
	}
}

void spe::UITopbar::build()
{
	// TODO REwrite with CMAke
	/*spe::Savesystem::SaveEverything(*this->m_ptr_Repo, *this->m_ptr_GUIRepo, *this->m_ptr_SceneHandler);

	const std::string PATH = spe::EngineData::s_path_to_user_project + "\\" + spe::EngineData::s_name_of_user_project;
	const std::filesystem::path TARGET_PATH = spe::EngineData::s_path_to_user_project + "\\" + spe::EngineData::s_name_of_user_project;
	const std::filesystem::path FILES_IN_FOLDER[FILE_AMOUNT] =
	{
		PATH_TO_USER_DEBUG_FOLDER"Assets.exe",
		PATH_TO_USER_DEBUG_FOLDER"sfml-audio-d-2.dll",
		PATH_TO_USER_DEBUG_FOLDER"sfml-graphics-d-2.dll",
		PATH_TO_USER_DEBUG_FOLDER"sfml-network-d-2.dll",
		PATH_TO_USER_DEBUG_FOLDER"sfml-system-d-2.dll",
		PATH_TO_USER_DEBUG_FOLDER"sfml-window-d-2.dll",
	};

	spe::flc::removeDir(PATH);
	std::filesystem::create_directories(TARGET_PATH);

	for (int i = 0; i < FILE_AMOUNT; i++)
	{
		const auto fileTarget = TARGET_PATH / FILES_IN_FOLDER[i].filename();

		if (std::filesystem::is_regular_file(FILES_IN_FOLDER[i]))
		{
			std::filesystem::copy_file(FILES_IN_FOLDER[i], fileTarget, std::filesystem::copy_options::overwrite_existing);
		}
	}

	spe::flc::copyDir(spe::EngineData::s_path_to_user_project + "\\assets", PATH, "\\assets", { "\\src\\", ".cpp", ".h" });
	spe::flc::copyDir(spe::EngineData::s_path_to_user_project + "\\engine", PATH, "\\engine", { "\\src\\", ".cpp", ".h" });*/
}

void spe::UITopbar::createScene()
{
	this->m_add_scene_mode = false;
	const std::string scene_name = std::string(this->m_new_scene_name);
	if (scene_name == "")
	{
		return;
	}
	this->m_ptr_SceneHandler->CreateScene(scene_name);
}

void spe::UITopbar::playGameButton()
{
	if (spe::Style::DisplaySmybolAsButton(ICON_FA_PLAY) || spe::Input::onKeyRelease(spe::KeyBoardCode::F5))
	{
		spe::Savesystem::SaveEverything(*this->m_ptr_Repo, *this->m_ptr_GUIRepo, *this->m_ptr_SceneHandler);

		const std::string engine = spe::Utility::GetCurrentDir();

		spe::Utility::SetCurrentDir(spe::EngineData::s_PathUserProject);

		const std::string path = "Build\\x64\\Debug\\" + spe::EngineData::s_NameOfUser + ".exe";
		system(path.c_str());

		spe::Utility::SetCurrentDir(engine);
	}
}

void spe::UITopbar::toolSelector()
{
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 28);
	for (int i = 0; i < TOOLS_AMOUNT; i++)
	{
		if (this->m_tools[i].background)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, REAL_EDITOR_BUTTON_BG_COLOR);
		}
		ImGui::SetCursorPosY(0);
		if (spe::Style::DisplaySmybolAsButton(this->m_tools[i].icon.c_str())) {}
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			this->m_clicked_on_btn = true;
			this->m_editor_tools = this->m_tools[i].tool;
			this->m_ptr_GUIRepo->Tools = this->m_editor_tools;
			this->m_update_event_to_set = true;
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

void spe::UITopbar::removeBackgroundFromButtons()
{
	for (int i = 0; i < TOOLS_AMOUNT; i++)
	{
		this->m_tools[i].background = false;
	}
}
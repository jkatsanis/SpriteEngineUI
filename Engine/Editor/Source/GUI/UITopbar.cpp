#include "UITopbar.h"

// Constructor

void spe::UITopbar::Init()
{
	this->m_DisplayInfo = true;
	this->m_SwitchSceneName = "";
	this->m_NewSceneName[0] = '\0';
	this->m_AddSceneMode = false;
	this->m_EditorTools = spe::EditorTools::PositionTool;
	this->Hovered = false;

	this->m_Tools[0] = spe::Tool(spe::EditorTools::PositionTool, ICON_FA_ARROWS, "Position");
	this->m_Tools[1] = spe::Tool(spe::EditorTools::ScaleTool, ICON_FA_PLUS, "Scaling");

	this->m_Tools[0].Background = true;
	this->m_ClickedOnBtn = true;
}

// Public functions

void spe::UITopbar::Render()
{
	this->Hovered = false;

	this->Hotkeys();
	this->RenderMainMenuBar();


	ImGui::Begin("##tools-buttons", NULL, DEFAULT_FLAGS);
	// Render
	ImGui::SetCursorPos(ImVec2(0, 0));
	this->PlayGameButton();
	this->ToolSelector();
	this->SimulateButton();

	if (ImGui::IsMouseReleased(0))
	{
		if (this->m_UpdateEventToSet)
		{
			this->m_UpdateEventToSet = false;
		}
	}

	if (!this->Hovered)
	{
		this->Hovered = spe::UIUtility::IsHovered(ImVec2(0, 0), ImVec2(1920, 50));
	}
	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
	
	ImGui::SetWindowPos(ImVec2(1920 - this->m_ptr_GUIRepo->InspectorData.ptr_Size->x - 120, 56));
	ImGui::SetWindowSize(ImVec2(120, 30));


	if (!this->Hovered)
	{
		this->Hovered = spe::UIUtility::IsHovered(ImGui::GetWindowPos(), ImVec2(120, 30));
	}
	ImGui::End();
}

// Private functions

void spe::UITopbar::RenderMainMenuBar()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1019f, .1019f, .1019f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, WINDOW_SIZE_Y_TOOL_BUTTONS));

	if (ImGui::BeginMainMenuBar())
	{
		spe::Style::RenderStyle();

		this->BuildProjectIntoFolder();
		this->RenderWindowSelecter();
		this->RenderToolSelector();
		this->ProjectSettings();
		this->RenderSceneSelector();
		this->DisplayEngineInfo();

		ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void spe::UITopbar::RenderToolSelector()
{
	if (ImGui::BeginMenu("Tools"))
	{
		this->Hovered = true;

		for (int i = 0; i < TOOLS_AMOUNT; i++)
		{
			if (ImGui::MenuItem(this->m_Tools[i].Name.c_str()))
			{
				this->m_EditorTools = this->m_Tools[i].tool;
				this->m_ptr_GUIRepo->Tools = this->m_EditorTools;
				this->RemoveBackgroundFromButtons();
				this->m_Tools[i].Background = true;
			}
		}
		ImGui::EndMenu();
	}
}

void spe::UITopbar::RenderSceneSelector()
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
				this->RemoveScene(name);
			}
			cursor.x += 27;
			cursor.y += 1.5f;
			identy += "symbol";
			if (spe::Style::DisplaySymbolAsButtonWithWidthAndCursorPos(ICON_FA_EDIT, cursor, ImVec2(30, 30), identy))
			{
				this->m_SwitchSceneName = name;
			}
			ImGui::Dummy(ImVec2(0, 5));
		}
		ImGui::Separator();
		if (ImGui::MenuItem("Add scene"))
		{
			this->m_AddSceneMode = true;
		}

		ImGui::EndMenu();
	}

	this->SwitchScene(this->m_SwitchSceneName);
	this->RenderSceneAddPopup();
}

void spe::UITopbar::RemoveScene(const std::string& scene)
{
	if (this->m_ptr_SceneHandler->TotalScenes.size() <= 1)
	{
		return;
	}
	for (size_t i = 0; i < this->m_ptr_SceneHandler->TotalScenes.size(); i++)
	{
		if (this->m_ptr_SceneHandler->TotalScenes.at(i) == scene)
		{
			this->m_ptr_SceneHandler->DeleteScene(scene);

			spe::Savesystem::UpdateSceneFile(*this->m_ptr_SceneHandler);

			if (this->m_ptr_SceneHandler->CurrentScene == scene)
			{
				this->m_ptr_SceneHandler->CurrentScene = this->m_ptr_SceneHandler->TotalScenes.at(0);
			}
		}
	}
}

void spe::UITopbar::RenderSceneAddPopup()
{
	if (!this->m_AddSceneMode)
	{
		return;
	}

	const static ImVec2 window_size = ImVec2(200, 75);

	if (ImGui::Begin("##create-scene", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
	{
		ImGui::Text("Add scene");
		ImGui::Separator();

		const ImVec2 cursor = ImGui::GetCursorPos();

		ImGui::SetCursorPos(ImVec2(cursor.x + 170, cursor.y - 45));
		if (ImGui::Button("x"))
		{
			this->m_AddSceneMode = false;
		}

		ImGui::SetCursorPos(cursor);

		ImGui::SetNextItemWidth(150);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 7));
		ImGui::InputTextWithHint("##add-scene", "<name>", this->m_NewSceneName, CHAR_MAX);

		ImGui::PopStyleVar();

		spe::UIUtility::SameLine(-10);

		if (spe::Style::DisplaySmybolAsButton(ICON_FA_ARROW_RIGHT))
		{
			this->CreateScene();
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
		this->m_AddSceneMode = false;
		this->m_NewSceneName[0] = '\0';
	}

	if (this->m_AddSceneMode && ImGui::IsKeyReleased(ImGuiKey_Enter))
	{
		this->CreateScene();
	}
}

void spe::UITopbar::SwitchScene(const std::string& scene)
{
	if (this->m_SwitchSceneName == "")
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
			spe::Savesystem::SaveEverything(*this->m_ptr_Repo, this->m_ptr_GUIRepo->Camera, this->m_ptr_GUIRepo->BackgroundColor, *this->m_ptr_SceneHandler);

			this->m_ptr_SceneHandler->LoadScene(scene, this->m_ptr_GUIRepo->Camera, this->m_ptr_GUIRepo->BackgroundColor);
			this->m_ptr_GUIRepo->CleanUp();
			this->m_ptr_GUIRepo->InitHierarchySprites(this->m_ptr_Repo->GetSprites());
			this->m_ptr_Repo->SortSpritesByLayer();
			this->m_SwitchSceneName = "";
		}
		ImGui::SameLine();
		if (ImGui::Button("Don't save"))
		{
			this->m_ptr_SceneHandler->LoadScene(scene, this->m_ptr_GUIRepo->Camera, this->m_ptr_GUIRepo->BackgroundColor);
			this->m_ptr_GUIRepo->CleanUp();
			this->m_ptr_GUIRepo->InitHierarchySprites(this->m_ptr_Repo->GetSprites());
			this->m_ptr_Repo->SortSpritesByLayer();

			this->m_SwitchSceneName = "";
		}
		if (ImGui::IsKeyReleased(ImGuiKey_Escape))
		{
			this->m_SwitchSceneName = "";
		}

		spe::UIUtility::SetWindowScreenMiddle(ImVec2(200, 50));
		ImGui::SetWindowSize(ImVec2(200, 50)),
			ImGui::End();
	}
}

void spe::UITopbar::DisplayEngineInfo()
{
	if (!this->m_DisplayInfo)
	{
		return;
	}

	const std::string scene_text = "Editing: " + this->m_ptr_SceneHandler->CurrentScene;
	ImGui::SetCursorPosX(1880 - ImGui::CalcTextSize(scene_text.c_str()).x);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
	ImGui::Text(scene_text.c_str());

	const std::string info_text = "FPS: " + std::to_string(spe::Time::s_FPS);
	ImGui::SetCursorPosX(1650 - ImGui::CalcTextSize(info_text.c_str()).x);
	ImGui::Text(info_text.c_str());

	this->SimulateButton();

	ImGui::PopStyleColor();
}

void spe::UITopbar::ProjectSettings()
{
	if (ImGui::BeginMenu("Project"))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 4));

		ImGui::Text("Build   debug");
		spe::UIUtility::SameLine(-1);
		ImGui::Checkbox("##debug", &spe::EngineData::s_BuildDebug);
		ImGui::Dummy(ImVec2(0, 2));

		ImGui::Text("Build release");
		spe::UIUtility::SameLine(-1);
		ImGui::Checkbox("##release", &spe::EngineData::s_BuildRelease);
		ImGui::Dummy(ImVec2(0, 5));

		ImGui::PopStyleVar();

		ImGui::EndMenu();
	}
}

void spe::UITopbar::SimulateButton()
{
	ImGui::SameLine();

	ImGui::SetCursorPosX(550);
	ImGui::SetCursorPosY(22);
	
	ImGui::SetWindowFontScale(0.75f);
	ImGui::Text("Warning: Will save the current state, and wont save anything in the simulation");
	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);

	ImGui::SameLine();
	ImGui::SetCursorPosX(700);

	if (!this->m_ptr_GUIRepo->SimulatePhysics)
	{
		if (spe::Style::DisplaySymbolInMenuItemWithText(ICON_FA_PLAY, "Simulate physics", 5))
		{
			spe::Savesystem::SaveEverything(*this->m_ptr_Repo, this->m_ptr_GUIRepo->Camera, this->m_ptr_GUIRepo->BackgroundColor, *this->m_ptr_SceneHandler);
			this->m_ptr_GUIRepo->SimulatePhysics = true;
			spe::Savesystem::s_CanSave = false;
		}
	}
	else
	{
		if (spe::Style::DisplaySymbolInMenuItemWithText(ICON_FA_SQUARE, "Simulate physics##2", 5))
		{
			spe::Savesystem::s_CanSave = true;

			this->m_ptr_GUIRepo->SimulatePhysics = false;

			this->m_ptr_SceneHandler->LoadScene(this->m_ptr_SceneHandler->CurrentScene, this->m_ptr_GUIRepo->Camera, this->m_ptr_GUIRepo->BackgroundColor);

			this->m_ptr_GUIRepo->CleanUp();
			this->m_ptr_GUIRepo->InitHierarchySprites(this->m_ptr_Repo->GetSprites());
			this->m_ptr_Repo->SortSpritesByLayer();
		}
	}
}

void spe::UITopbar::BuildProjectIntoFolder()
{
	if (ImGui::BeginMenu("File"))
	{
		this->Hovered = true;
		if (ImGui::MenuItem("Save", "CTRL + S"))
		{
			spe::Savesystem::SaveEverything(*this->m_ptr_Repo, this->m_ptr_GUIRepo->Camera, this->m_ptr_GUIRepo->BackgroundColor, *this->m_ptr_SceneHandler);
		}
		if (ImGui::MenuItem("Build", "CTRL + B"))
		{
			spe::EngineData::BuildProject();
		}

		ImGui::EndMenu();
	}
}

void spe::UITopbar::RenderWindowSelecter()
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
			this->m_DisplayInfo = !this->m_DisplayInfo;
		}
		ImGui::EndMenu();
	}
}

void spe::UITopbar::Hotkeys()
{
	if (spe::Input::OnKeyHold(spe::KeyBoardCode::LControl)
		&& spe::Input::OnKeyPress(spe::KeyBoardCode::S))
	{
		spe::Savesystem::SaveEverything(*this->m_ptr_Repo, this->m_ptr_GUIRepo->Camera, this->m_ptr_GUIRepo->BackgroundColor, *this->m_ptr_SceneHandler);
	}

	if (spe::Input::OnKeyHold(spe::KeyBoardCode::LControl)
		&& spe::Input::OnKeyPress(spe::KeyBoardCode::B))
	{
		spe::EngineData::BuildProject();
	}
}

void spe::UITopbar::CreateScene()
{
	this->m_AddSceneMode = false;
	const std::string scene_name = std::string(this->m_NewSceneName);
	if (scene_name == "")
	{
		return;
	}
	if (scene_name == "Template")
	{
		spe::Log::LogString("Cant create a scene with name 'Template'");
		return;
	}
	this->m_ptr_SceneHandler->CreateScene(scene_name);

	spe::Savesystem::UpdateSceneFile(*this->m_ptr_SceneHandler);
}

void spe::UITopbar::PlayGameButton()
{
	if (spe::Style::DisplaySmybolAsButton(ICON_FA_PLAY) || spe::Input::OnKeyRelease(spe::KeyBoardCode::F5))
	{
		spe::Savesystem::SaveEverything(*this->m_ptr_Repo, this->m_ptr_GUIRepo->Camera, this->m_ptr_GUIRepo->BackgroundColor, *this->m_ptr_SceneHandler);

		spe::EngineData::BuildProject();

		const std::string current1 = spe::Utility::GetCurrentDir();

		const std::string path = "Build";
		const std::string exe = "Debug\\" + spe::EngineData::s_NameOfUser + ".exe";
		const std::string current = spe::Utility::GetCurrentDir();

		spe::Utility::SetCurrentDir(path);

		// Need to go in the path of the user because the way of the user project is setup (Bad design ik ik)
		system(exe.c_str());

		spe::Utility::SetCurrentDir(current);
	}
}

void spe::UITopbar::ToolSelector()
{
	const float x = ImGui::GetCursorPosX() + 28;

	ImGui::SetCursorPosX(x);
	for (int i = 0; i < TOOLS_AMOUNT; i++)
	{
		if (this->m_Tools[i].Background)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, REAL_EDITOR_BUTTON_BG_COLOR);
		}
		ImGui::SetCursorPosY(0);
		if (spe::Style::DisplaySmybolAsButton(this->m_Tools[i].Icon.c_str())) {}
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			this->m_ClickedOnBtn = true;
			this->m_EditorTools = this->m_Tools[i].tool;
			this->m_ptr_GUIRepo->Tools = this->m_EditorTools;
			this->m_UpdateEventToSet = true;
		}
		if (this->m_Tools[i].Background)
		{
			ImGui::PopStyleColor();
		}
		if (this->m_ClickedOnBtn)
		{
			this->RemoveBackgroundFromButtons();
			this->m_ClickedOnBtn = false;
			this->m_Tools[i].Background = true;
		}
		ImGui::SameLine();
	}
}

void spe::UITopbar::RemoveBackgroundFromButtons()
{
	for (int i = 0; i < TOOLS_AMOUNT; i++)
	{
		this->m_Tools[i].Background = false;
	}
}
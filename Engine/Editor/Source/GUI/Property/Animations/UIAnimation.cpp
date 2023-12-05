#include "UIAnimation.h"

//Public functions

void spe::UIAnimation::Init()
{
	this->m_AnimationFile[0] = 0;
	this->m_FileName = "";
	const std::string pathToAssets = "Assets\\";
	this->m_OpenFileDialoge = spe::FileDialog(pathToAssets, ICON_FA_PLUS, "Open Animation", ImVec2(500, 250), true, spe::Style::s_DefaultFontSize);
	this->m_CreateFileDialoge = spe::FileDialog(pathToAssets, ICON_FA_PLUS, "Create Animation", ImVec2(500, 250), false, spe::Style::s_DefaultFontSize);
	this->m_CreateFileDialoge.SetFirstNode("assets");
	this->m_OpenFileDialoge.SetFirstNode("assets");
	this->m_BackgroundCounter = START_CNT_BG;

	this->m_ptr_GUIRepo->AnimationData.IsOpen = false;

	this->m_UIAnimationEditor.SetRepos(this->m_ptr_Repo, this->m_ptr_GUIRepo);
}

void spe::UIAnimation::Render()
{
	if (!this->m_ptr_GUIRepo->AnimationData.IsOpen || this->m_ptr_GUIRepo->sprite_in_inspector == nullptr)
	{
		this->Hovered = false;
		return;
	}
	if (this->m_UIAnimationEditor.Display)
	{
		this->m_UIAnimationEditor.Render();
		this->Hovered = this->m_UIAnimationEditor.Hovered;
	}
	else
	{
		this->m_BackgroundCounter = START_CNT_BG;
		ImGui::Begin("##Animations", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

		this->m_ptr_GUIRepo->AnimationData.IsOpen = spe::UIUtility::RenderCloseRectangle(
			50, ICON_FA_FILE_CODE, "##close-rectangle-animation", "Animation", 0);
		this->RenderAnimationUIOptions();

		if (this->m_ptr_GUIRepo->AnimationData.Reload)
		{
			this->m_ptr_GUIRepo->AnimationData.Reload = false;
			spe::UIUtility::SetWindowScreenMiddle(WINDOW_SIZE_ANIMATION_CREATE);
		}
		this->GetFileNameInput();
		this->DisplayAnimations();

		ImGui::SetWindowSize(WINDOW_SIZE_ANIMATION_CREATE);

		ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);

		if (!this->Hovered)
			this->Hovered = spe::UIUtility::IsHovered(ImGui::GetWindowPos(), ImGui::GetWindowSize());
		ImGui::End();
	}
}

// Private functions

void spe::UIAnimation::GetFileNameInput()
{
	const ImVec2 pos = ImGui::GetCursorPos();

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 250, ImGui::GetCursorPosY() + 57));

    ImVec2 open_pos = ImVec2(ImGui::GetCursorPosX() + 30, ImGui::GetCursorPosY() - 54);

	ImGui::SetCursorPos(open_pos);
	ImGui::Text("Open animation");

	open_pos.x -= 30;

	ImGui::SetCursorPos(open_pos);
	if (spe::Style::DisplaySmybolAsButton(ICON_FA_PLUS))
	{
		this->m_OpenFileDialoge.EnableWindow("Open animation");
	}

	ImGui::SetCursorPos(pos);

	this->m_OpenFileDialoge.Update();

	////////////////////
	// Handling the open_file_dialog
	if (this->m_OpenFileDialoge.IsItemSelected())
	{
		spe::Sprite* sprite = this->m_ptr_GUIRepo->sprite_in_inspector;
		// Loading the sprites from the user directory
		const std::string ext = "." + spe::Utility::GetFileExtension(this->m_OpenFileDialoge.PathClicked);
		if (ext == EXTENSION_ANIMATION_FILE)
		{
			spe::Initializer::InitAnimation(this->m_OpenFileDialoge.PathClicked, sprite);
			// Updating the file here because a new animation got added
		}
		else
		{
			spe::Log::LogString("Animation was not in the right format!");
		}

		this->m_OpenFileDialoge.DisableWindow();
	}

	const std::string icon_2 = ICON_FA_PLUS + std::string("##ADD");
	// Open popup
	if (spe::Style::DisplaySmybolAsButton(icon_2.c_str()))
	{
		this->m_CreateFileDialoge.EnableWindow();
	}

	ImGui::SameLine();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);


	ImGui::Text("Add animation");

	this->m_CreateFileDialoge.Update();
	
	////////////////////
	// Handling the creat_file_dialog
	if (this->m_CreateFileDialoge.PathClicked == "")
	{
		return;
	}

	ImGui::SetNextWindowFocus();
	if (ImGui::Begin("##CreateAnimtion", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
	{
		ImVec2 old = ImGui::GetCursorPos();
		ImVec2 closeCursorPos = ImVec2(WINDOW_SIZE_ANIMATION_CREATE.x - 50, ImGui::GetCursorPosY() - 5);
		ImGui::SetCursorPos(closeCursorPos);

		// Clicked at the "x", stop displaying the file dialoge
		if (ImGui::Button("x") || spe::Input::OnKeyPress(spe::KeyBoardCode::Escape))
		{
			this->m_AnimationFile[0] = '\0';
			this->m_CreateFileDialoge.DisableWindow();
		}
		ImGui::SetCursorPos(old);

		ImGui::InputTextWithHint("##addFile", "<name>", this->m_AnimationFile, CHAR_MAX);
		if (ImGui::IsItemHovered() || ImGui::IsItemFocused())
		{
			this->Hovered = true;
		}
		const std::string path = spe::Utility::getUserProjectPathSeperatetFromEnginePath(this->m_CreateFileDialoge.PathClicked);
		std::string createAnimtionAt = "Create animation file at: " + path;
		ImGui::Text(createAnimtionAt.c_str());
		ImGui::Text("Press Enter when u are done giving it a name");

		if(!this->Hovered)
			this->Hovered = spe::UIUtility::IsHovered(ImGui::GetWindowPos(), ImVec2(WINDOW_SIZE_ANIMATION_CREATE.x, 120));

		ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
		ImGui::SetWindowSize(ImVec2(WINDOW_SIZE_ANIMATION_CREATE.x, 120));
		ImGui::End();
	}
	this->AddAnimationsToAnimator();
}

void spe::UIAnimation::DisplayAnimations()
{
	ImGui::Dummy(ImVec2(0, 5)); 
	static float s_ExistCheckCounter = 0.0f;

	s_ExistCheckCounter += spe::Time::s_DeltaTime;

	for (auto& anim : this->m_ptr_GUIRepo->sprite_in_inspector->Animator.Animations)
	{
		const std::string& key = anim.first;

		if (!this->m_FilterSearch.PassFilter(key.c_str()))
		{
			continue;
		}
		this->DrawBackgroundBehinAnimation();
		if (ImGui::Selectable(anim.second.GetName().c_str(), false, ImGuiSelectableFlags_DontClosePopups,
			ImVec2(ImGui::CalcTextSize(anim.second.GetName().c_str()).x, 0)))
		{
			this->EnterAnimation(anim.second);
			break;
		}

		const std::string s = "#" + anim.second.GetName();
		const std::string deleteButton = ICON_FA_TRASH "##" + anim.second.GetName();

		bool file_exist = true;
		if (s_ExistCheckCounter >= 0.2f)
		{
			file_exist = std::filesystem::exists(anim.second.GetPath());
			s_ExistCheckCounter = 0.0f;
		}
		if (spe::Style::DisplaySmybolAsButton(deleteButton.c_str(),
			ImVec2(WINDOW_SIZE_ANIMATION_CREATE.x - 50, ImGui::GetCursorPosY() - 30))
			|| !file_exist)
		{
			if (!file_exist)
			{
				spe::Log::LogString("Animation file doesnt exist -> deleting from list..");
			}
			this->m_ptr_GUIRepo->sprite_in_inspector->Animator.RemoveAnimation(anim.second.GetName());
			break;
		}
	}
}

void spe::UIAnimation::EnterAnimation(spe::Animation& animation)
{
	this->m_UIAnimationEditor.SetAnim(&animation);
	this->m_UIAnimationEditor.Render();
}

void spe::UIAnimation::DrawBackgroundBehinAnimation()
{
	if (this->m_BackgroundCounter < 1)
	{
		this->m_BackgroundCounter++;
		return;
	}
	this->m_BackgroundCounter = 0;

	const ImVec2 temp = ImGui::GetCursorPos();
	ImGui::SetCursorPosX(ImGui::GetWindowPos().x);
	ImGui::SetCursorPosY(ImGui::GetWindowPos().y + ImGui::GetCursorPosY() + 23);
	spe::UIUtility::DrawRectangleInGUIWIndow(ImVec2(WINDOW_SIZE_ANIMATION_CREATE.x, 24), ImGui::GetCursorPos(), COMPONENT_SELECTED_COLOR);
	ImGui::SetCursorPos(temp);
}

void spe::UIAnimation::RenderAnimationUIOptions() noexcept
{
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
	ImGui::SetCursorPosX(0);
	ImGui::BeginChild("##animations-options-container", ImVec2(WINDOW_SIZE_ANIMATION_CREATE.x, 45));

	ImGui::SetCursorPos(ImVec2(0, ImGui::GetCursorPosY() + 10));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f); // Set rounding to 5 pixels
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 3.0f)); // Add some padding for visual clarity
	ImGui::SetNextItemWidth(150);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50);
	this->m_FilterSearch.Draw("Search");
	ImGui::PopStyleVar(2);

	ImGui::EndChild();
}

void spe::UIAnimation::AddAnimationsToAnimator()
{
	if (this->m_CreateFileDialoge.PathClicked != "" && ImGui::IsKeyReleased(ImGuiKey_Enter))
	{
		for (std::pair<std::string, spe::Animation> anim : this->m_ptr_GUIRepo->sprite_in_inspector->Animator.Animations)
		{
			// Found a aniamtion with this name, which already exists
			if (anim.first == this->m_AnimationFile)
			{
				spe::Log::LogString(" Cant create anoter file with the same name!");
				return;
			}
		}
		if (this->m_AnimationFile[0] != '\0')
		{
			const std::string& path = 
				this->m_CreateFileDialoge.PathClicked
				+ this->m_AnimationFile
				+ EXTENSION_ANIMATION_FILE;
			this->m_ptr_GUIRepo->sprite_in_inspector->Animator.CreateAnimation(this->m_AnimationFile, path, { });
		}
		spe::Savesystem::CreateAnimationSaveFile
			(this->m_ptr_GUIRepo->sprite_in_inspector, this->m_ptr_GUIRepo->sprite_in_inspector->Animator.Animations[this->m_AnimationFile]);

		this->m_CreateFileDialoge.DisableWindow();
		this->m_AnimationFile[0] = '\0';
	}
}
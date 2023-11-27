#include "UIAnimation.h"

//Public functions

void spe::UIAnimation::Init()
{
	this->m_animationFile[0] = 0;
	this->m_fileName = "";
	const std::string pathToAssets = "Assets\\";
	this->m_animation_open_file_dialog = spe::FileDialog(pathToAssets, ICON_FA_PLUS, "Open Animation", ImVec2(500, 250), true, spe::Style::s_DefaultFontSize);
	this->m_animation_create_file_dialog = spe::FileDialog(pathToAssets, ICON_FA_PLUS, "Create Animation", ImVec2(500, 250), false, spe::Style::s_DefaultFontSize);
	this->m_animation_create_file_dialog.setFirstNode("assets");
	this->m_animation_open_file_dialog.setFirstNode("assets");
	this->m_background_counter = START_CNT_BG;

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
	if (this->m_UIAnimationEditor.display)
	{
		this->m_UIAnimationEditor.Render();
		this->Hovered = this->m_UIAnimationEditor.Hovered;
	}
	else
	{
		this->m_background_counter = START_CNT_BG;
		ImGui::Begin("##Animations", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

		this->m_ptr_GUIRepo->AnimationData.IsOpen = spe::UIUtility::RenderCloseRectangle(
			50, ICON_FA_FILE_CODE, "##close-rectangle-animation", "Animation", 0);
		this->renderAnimationUIOptions();

		if (this->m_ptr_GUIRepo->AnimationData.Reload)
		{
			this->m_ptr_GUIRepo->AnimationData.Reload = false;
			spe::UIUtility::SetWindowScreenMiddle(WINDOW_SIZE_ANIMATION_CREATE);
		}
		this->getFileNameInput();
		this->displayAnimations();

		ImGui::SetWindowSize(WINDOW_SIZE_ANIMATION_CREATE);

		ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);

		if (!this->Hovered)
			this->Hovered = spe::UIUtility::IsHovered(ImGui::GetWindowPos(), ImGui::GetWindowSize());
		ImGui::End();
	}
}

// Private functions

void spe::UIAnimation::getFileNameInput()
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
		this->m_animation_open_file_dialog.enableWindow("Open animation");
	}

	ImGui::SetCursorPos(pos);

	this->m_animation_open_file_dialog.update();

	////////////////////
	// Handling the open_file_dialog
	if (this->m_animation_open_file_dialog.IsItemSelected())
	{
		spe::Sprite* sprite = this->m_ptr_GUIRepo->sprite_in_inspector;
		// Loading the sprites from the user directory
		const std::string ext = "." + spe::Utility::GetFileExtension(this->m_animation_open_file_dialog.pathClicked);
		if (ext == EXTENSION_ANIMATION_FILE)
		{
			spe::Initializer::InitAnimation(this->m_animation_open_file_dialog.pathClicked, sprite);
			// Updating the file here because a new animation got added
			spe::Savesystem::UpdateKnownAnimationFile(this->m_ptr_Repo);
		}
		else
		{
			spe::Log::LogString("Animation was not in the right format!");
		}

		this->m_animation_open_file_dialog.disableWindow();
	}

	const std::string icon_2 = ICON_FA_PLUS + std::string("##ADD");
	// Open popup
	if (spe::Style::DisplaySmybolAsButton(icon_2.c_str()))
	{
		this->m_animation_create_file_dialog.enableWindow();
	}

	ImGui::SameLine();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);


	ImGui::Text("Add animation");

	this->m_animation_create_file_dialog.update();
	
	////////////////////
	// Handling the creat_file_dialog
	if (this->m_animation_create_file_dialog.pathClicked == "")
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
		if (ImGui::Button("x") || spe::Input::onKeyPress(spe::KeyBoardCode::Escape))
		{
			this->m_animationFile[0] = '\0';
			this->m_animation_create_file_dialog.disableWindow();
		}
		ImGui::SetCursorPos(old);

		ImGui::InputTextWithHint("##addFile", "<name>", this->m_animationFile, CHAR_MAX);
		if (ImGui::IsItemHovered() || ImGui::IsItemFocused())
		{
			this->Hovered = true;
		}
		const std::string path = spe::Utility::getUserProjectPathSeperatetFromEnginePath(this->m_animation_create_file_dialog.pathClicked);
		std::string createAnimtionAt = "Create animation file at: " + path;
		ImGui::Text(createAnimtionAt.c_str());
		ImGui::Text("Press Enter when u are done giving it a name");

		if(!this->Hovered)
			this->Hovered = spe::UIUtility::IsHovered(ImGui::GetWindowPos(), ImVec2(WINDOW_SIZE_ANIMATION_CREATE.x, 120));

		ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
		ImGui::SetWindowSize(ImVec2(WINDOW_SIZE_ANIMATION_CREATE.x, 120));
		ImGui::End();
	}
	this->addAnimationsToAnimator();
}

void spe::UIAnimation::displayAnimations()
{
	ImGui::Dummy(ImVec2(0, 5)); 
	static float s_ExistCheckCounter = 0.0f;

	s_ExistCheckCounter += spe::Time::s_delta_time;

	for (auto& anim : this->m_ptr_GUIRepo->sprite_in_inspector->animator.animations)
	{
		const std::string& key = anim.first;

		if (!this->m_search_filter_animation.PassFilter(key.c_str()))
		{
			continue;
		}
		this->drawBackgroundBehinAnimation();
		if (ImGui::Selectable(anim.second.GetName().c_str(), false, ImGuiSelectableFlags_DontClosePopups,
			ImVec2(ImGui::CalcTextSize(anim.second.GetName().c_str()).x, 0)))
		{
			this->enterAnimation(anim.second);
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
			this->m_ptr_GUIRepo->sprite_in_inspector->animator.removeAnimation(anim.second.GetName());
			spe::Savesystem::UpdateKnownAnimationFile(this->m_ptr_Repo);
			break;
		}
	}
}

void spe::UIAnimation::enterAnimation(spe::Animation& animation)
{
	this->m_UIAnimationEditor.setAnim(&animation);
	this->m_UIAnimationEditor.Render();
}

void spe::UIAnimation::drawBackgroundBehinAnimation()
{
	if (this->m_background_counter < 1)
	{
		this->m_background_counter++;
		return;
	}
	this->m_background_counter = 0;

	const ImVec2 temp = ImGui::GetCursorPos();
	ImGui::SetCursorPosX(ImGui::GetWindowPos().x);
	ImGui::SetCursorPosY(ImGui::GetWindowPos().y + ImGui::GetCursorPosY() + 23);
	spe::UIUtility::DrawRectangleInGUIWIndow(ImVec2(WINDOW_SIZE_ANIMATION_CREATE.x, 24), ImGui::GetCursorPos(), COMPONENT_SELECTED_COLOR);
	ImGui::SetCursorPos(temp);
}

void spe::UIAnimation::renderAnimationUIOptions()
{
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
	ImGui::SetCursorPosX(0);
	ImGui::BeginChild("##animations-options-container", ImVec2(WINDOW_SIZE_ANIMATION_CREATE.x, 45));

	ImGui::SetCursorPos(ImVec2(0, ImGui::GetCursorPosY() + 10));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f); // Set rounding to 5 pixels
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 3.0f)); // Add some padding for visual clarity
	ImGui::SetNextItemWidth(150);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50);
	this->m_search_filter_animation.Draw("Search");
	ImGui::PopStyleVar(2);

	ImGui::EndChild();
}

void spe::UIAnimation::addAnimationsToAnimator()
{
	if (this->m_animation_create_file_dialog.pathClicked != "" && ImGui::IsKeyReleased(ImGuiKey_Enter))
	{
		for (std::pair<std::string, spe::Animation> anim : this->m_ptr_GUIRepo->sprite_in_inspector->animator.animations)
		{
			// Found a aniamtion with this name, which already exists
			if (anim.first == this->m_animationFile)
			{
				spe::Log::LogString(" Cant create anoter file with the same name!");
				return;
			}
		}
		if (this->m_animationFile[0] != '\0')
		{
			const std::string& path = 
				this->m_animation_create_file_dialog.pathClicked
				+ this->m_animationFile
				+ EXTENSION_ANIMATION_FILE;
			this->m_ptr_GUIRepo->sprite_in_inspector->animator.createAnimation(this->m_animationFile, path, { });
		}
		spe::Savesystem::CreateAnimationSaveFile
			(this->m_ptr_GUIRepo->sprite_in_inspector, this->m_ptr_GUIRepo->sprite_in_inspector->animator.animations[this->m_animationFile], this->m_ptr_Repo);

		this->m_animation_create_file_dialog.disableWindow();
		this->m_animationFile[0] = '\0';
	}
}
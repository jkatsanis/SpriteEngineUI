#include "UIAnimation.h"

s2d::UIAnimation::UIAnimation()
{
	this->m_UIAnimationEditor.resetAnim();
	this->m_fileName = "";
	const std::string pathToAssets = s2d::EngineData::s_path_to_user_project + "\\" + "assets\\";
	this->m_createAnimtionPathFileDialoge = s2d::FileDialog(pathToAssets, ICON_FA_PLUS, "Create Animation", ImVec2(500, 250));
	this->m_createAnimtionPathFileDialoge.setFirstNode("assets");
}

//Public functions

void s2d::UIAnimation::createUIAnimationWindow()
{
	if (!s2d::UIInfo::s_is_animation_open.is_open)
	{
		return;
	}
	if (this->m_UIAnimationEditor.display)
	{
		this->m_UIAnimationEditor.displayEditor();
		this->isHovered = this->m_UIAnimationEditor.is_hovered;
	}
	else
	{
		ImGui::Begin("##Animations", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		this->displayTopOfEditor();
		this->getFileNameInput();
		this->displayAnimations();
		this->addAnimationsToAnimator();

		ImGui::SetWindowSize(ImVec2(500, 500));

		if(!this->isHovered)
			this->isHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenBlockedByPopup);

		ImGui::End();
	}
}

void s2d::UIAnimation::setSpriteRepository(s2d::SpriteRepository& repo)
{
	this->m_ptr_repo = &repo;
	this->m_UIAnimationEditor.setSpriteRepository(repo);
}

//Private functions

void s2d::UIAnimation::getFileNameInput()
{
	//Open popup
	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_PLUS, s2d::UIInfo::s_default_font_size - 0.2f))
	{
		this->m_openFileDialog = true;
		this->m_createAnimtionPathFileDialoge.enableWindow();
		this->m_openInputWindow = true;
	}

	ImGui::SameLine();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
	ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size - 0.2f);
	ImGui::Text("Add animations");
	ImGui::Separator();
	ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);

	if (this->m_openFileDialog)
	{
		if (this->m_createAnimtionPathFileDialoge.pathClicked == ""
			&& !this->m_createAnimtionPathFileDialoge.closeWindow())
		{
			this->m_createAnimtionPathFileDialoge.enableWindow();
			this->m_createAnimtionPathFileDialoge.displayNodes();
		}
	}

	if (this->m_createAnimtionPathFileDialoge.pathClicked == "")
	{
		return;
	}

	if(ImGui::Begin("##CreateAnimtion", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
	{
		ImVec2 old = ImGui::GetCursorPos();
		ImVec2 closeCursorPos = ImVec2(ImGui::GetCursorPosX() + 250, ImGui::GetCursorPosY() - 5);
		ImGui::SetCursorPos(closeCursorPos);

		// Clicked at the "x", stop displaying the file dialoge
		if (ImGui::Button("x") || s2d::Input::onKeyPress(s2d::KeyBoardCode::Escape))
		{
			this->m_openInputWindow = false;
			this->m_openFileDialog = false;
			this->m_animationFile[0] = '\0';
			s2d::UIInfo::s_is_animation_open.is_open = false;
			this->m_createAnimtionPathFileDialoge.disableWindow();
		}
		ImGui::SetCursorPos(old);

		ImGui::InputTextWithHint("##addFile", "<name>", this->m_animationFile, CHAR_MAX);
		if (ImGui::IsItemHovered() || ImGui::IsItemFocused())
		{
			this->isHovered = true;
		}
		const std::string path = s2d::UI::getUserProjectPathSeperatetFromEnginePath(this->m_createAnimtionPathFileDialoge.pathClicked);
		std::string createAnimtionAt = "Create animation file at: " + path;
		ImGui::Text(createAnimtionAt.c_str());
		ImGui::Text("Press Enter when u are done giving it a name");
		ImGui::SetWindowSize(ImVec2(500, 120));
		ImGui::End();
	}
}

void s2d::UIAnimation::displayAnimations()
{
	for (auto& anim : this->m_ptr_repo->sprite_in_inspector->animator.animations)
	{
		ImGui::Text(anim.second.name.c_str());
		ImGui::SetCursorPos(ImVec2(455, ImGui::GetCursorPosY() - 32.5f));
		std::string s = "#" + anim.second.name;
		std::string button = ICON_FA_EDIT "##" + anim.second.name;
		std::string deleteButton = ICON_FA_TRASH "##" + anim.second.name;

		if (s2d::FontManager::displaySmybolAsButton(button.c_str()))
		{
			// setting animation for the UIAnimationEditor
			this->m_UIAnimationEditor.setAnim(&anim.second);
			this->m_UIAnimationEditor.displayEditor();
		}
		if (s2d::FontManager::displaySmybolAsButton(deleteButton.c_str(),
			ImVec2(ImGui::GetCursorPosX() + 410, ImGui::GetCursorPosY() - 37)))
		{
			
		}
	}
	
}

void s2d::UIAnimation::displayTopOfEditor()
{
	ImGui::Text("Animations");
	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 345 , ImGui::GetCursorPosY() - 10));

	//Close button
	if (ImGui::Button("x"))
	{
		s2d::UIInfo::s_render_asset_folder = true;
	}

	ImGui::Separator();
}

void s2d::UIAnimation::addAnimationsToAnimator()
{
	if (this->m_openInputWindow && ImGui::IsKeyReleased(ImGuiKey_Enter))
	{
		if (this->m_animationFile[0] != '\0')
		{
			const std::string& path = 
				s2d::UI::getUserProjectPathSeperatetFromEnginePath(this->m_createAnimtionPathFileDialoge.pathClicked)
				+ this->m_animationFile
				+ EXTENSION_ANIMATION_FILE;
			this->m_ptr_repo->sprite_in_inspector->animator.createAnimation(this->m_animationFile, path, { });
		}
		this->m_createAnimtionPathFileDialoge.disableWindow();
		this->m_openFileDialog = false;
		this->m_animationFile[0] = '\0';
	}
}

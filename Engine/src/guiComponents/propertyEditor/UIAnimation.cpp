#include "UIAnimation.h"

s2d::UIAnimation::UIAnimation()
{
	this->m_UIAnimationEditor.resetAnim();
	this->m_file_name = "";
	const std::string pathToAssets = s2d::EngineData::s_pathToUserProject + "\\" + "assets\\";
	this->m_createAnimtionPathFileDialoge = s2d::FileDialog(pathToAssets, ICON_FA_PLUS, "Create Animation", ImVec2(500, 250));
	this->m_createAnimtionPathFileDialoge.setFirstNode("assets");
}

//Public functions

void s2d::UIAnimation::createUIAnimationWindow()
{
	if (s2d::UIInfo::srenderAssetFolder || s2d::UIHirachy::selectedSprite == nullptr)
	{
		this->isHovered = false;
		return;
	}	

	if (this->m_UIAnimationEditor.display)
	{
		this->m_UIAnimationEditor.displayEditor();
		this->isHovered = this->m_UIAnimationEditor.isHovered;
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


//Private functions

void s2d::UIAnimation::getFileNameInput()
{
	//Open popup
	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_PLUS, s2d::UIInfo::sdefaultFontSize - 0.2f))
	{
		this->m_openFileDialog = true;
		this->m_createAnimtionPathFileDialoge.enableWindow();
		this->m_openInputWindow = true;
	}

	ImGui::SameLine();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
	ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize - 0.2f);
	ImGui::Text("Add animations");
	ImGui::Separator();
	ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);

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
			this->mogus[0] = '\0';
			this->m_createAnimtionPathFileDialoge.disableWindow();
		}
		ImGui::SetCursorPos(old);

		ImGui::InputTextWithHint("##addFile", "<name>", this->mogus, CHAR_MAX);
		if (ImGui::IsItemHovered() || ImGui::IsItemFocused())
		{
			this->isHovered = true;
		}
		const std::string path = s2d::UI::getUserProjectPathSeperatetFromEnginePath(this->m_createAnimtionPathFileDialoge.pathClicked);
		std::string createAnimtionAt = "Create animation file at: " + path;
		ImGui::Text(createAnimtionAt.c_str());
		
		ImGui::End();
	}
}

void s2d::UIAnimation::displayAnimations()
{
	for (s2d::Animation& anim : s2d::UIHirachy::selectedSprite->animator.animations)
	{
		ImGui::Text(anim.name.c_str());
		ImGui::SetCursorPos(ImVec2(455, ImGui::GetCursorPosY() - 32.5f));
		std::string s = "#" + anim.name;
		std::string button = ICON_FA_EDIT "##" + anim.name;

		if (s2d::FontManager::displaySmybolAsButton(button.c_str()))
		{
			// setting animation for the UIAnimationEditor
			this->m_UIAnimationEditor.setAnim(&anim);
			this->m_UIAnimationEditor.displayEditor();
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
		s2d::UIInfo::srenderAssetFolder = true;
	}

	ImGui::Separator();
}

void s2d::UIAnimation::addAnimationsToAnimator()
{
	if (this->m_openInputWindow && ImGui::IsKeyReleased(ImGuiKey_Enter))
	{
		s2d::UIHirachy::selectedSprite->animator.createAnimation(this->mogus, this->m_createAnimtionPathFileDialoge.pathClicked, { }, true);
		this->m_createAnimtionPathFileDialoge.disableWindow();
		this->m_openFileDialog = false;
		this->mogus[0] = '\0';
	}
}

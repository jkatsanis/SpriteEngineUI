#include "UIAnimation.h"

s2d::UIAnimation::UIAnimation()
{
	this->m_UIAnimationEditor.anim = nullptr;
	this->m_UIAnimationEditor.display = false;
	this->m_file_name = "";
}

//Public functions

void s2d::UIAnimation::createUIAnimationWindow()
{
	if (s2d::UIInfo::srenderAssetFolder || s2d::UIHirachy::selectedSprite == nullptr)
	{
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

		ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
		ImGui::SetWindowSize(ImVec2(500, 500));

		this->isHovered = ImGui::IsWindowHovered();

		ImGui::End();
	}
	std::cout << isHovered << std::endl;
}


//Private functions

void s2d::UIAnimation::getFileNameInput()
{
	//Open popup
	if (ImGui::Button(ICON_FA_PLUS))
	{
		ImGui::OpenPopup("Lol");
	}

	ImGui::SameLine();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
	ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize - 0.2f);
	ImGui::Text("Add animations");
	ImGui::Separator();
	ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);

	ImVec2 animation_pos = ImGui::GetCursorScreenPos();
	ImGui::SetNextWindowPos(ImVec2(animation_pos.x + 22, animation_pos.y + 50));

	//Popup input
	if (ImGui::BeginPopup("Lol"))
	{
		ImGui::InputTextWithHint("##addFile", "<name>", this->mogus, CHAR_MAX);

		ImGui::EndPopup();
	}
}

void s2d::UIAnimation::displayAnimations()
{
	for (s2d::Animation& anim : s2d::UIHirachy::selectedSprite->animator.animations)
	{
		ImGui::Text(anim.name.c_str());
		ImGui::SetCursorPos(ImVec2(455, ImGui::GetCursorPosY() - 32.5f));
		std::string s = "#" + anim.name;
		std::string button = ICON_FA_EDIT "    ##" + anim.name;

		if (ImGui::Button(button.c_str()))
		{

			this->m_UIAnimationEditor.anim = &anim;
			this->m_UIAnimationEditor.displayEditor();
		}
	}
}

void s2d::UIAnimation::displayTopOfEditor()
{
	ImGui::Text("Animations");
	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 300, ImGui::GetCursorPosY() - 10));

	//Close button
	if (ImGui::Button("x"))
	{
		s2d::UIInfo::srenderAssetFolder = true;
	}

	ImGui::Separator();
}

void s2d::UIAnimation::addAnimationsToAnimator()
{
	if (!ImGui::IsPopupOpen("Lol"))
	{
	}

	if (ImGui::IsPopupOpen("Lol") && ImGui::IsKeyReleased(ImGuiKey_Enter))
	{
		s2d::UIHirachy::selectedSprite->animator.createAnimation(this->mogus, { "d", "d" }, 500, true);
	}
}

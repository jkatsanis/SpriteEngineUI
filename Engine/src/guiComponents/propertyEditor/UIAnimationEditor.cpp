#include "UIAnimationEditor.h"

//Constructor

s2d::UIAnimationEditor::UIAnimationEditor()
{
	// 100 by default, will increase with the animation size
	this->m_keyFramesToEdit = 100;
	this->editor = s2d::UIAnimationKeyFrameEditor();
	this->anim = nullptr;
	this->display = false;
	this->isHovered = false;
}

//Private methods

void s2d::UIAnimationEditor::closeWindow()
{
	std::string name = "Editor - " + this->anim->name;

	ImGui::SetCursorPos(ImVec2(8 + ImGui::GetScrollX(), 15));
	ImGui::Text(name.c_str());

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 850 + ImGui::GetScrollX(), ImGui::GetCursorPosY() - 38));

	//Close button
	if (ImGui::Button("x"))
	{
		this->display = false;
		this->anim = nullptr;
	}

	if (!this->isHovered)
		this->isHovered = ImGui::IsWindowHovered();

	ImGui::End();
}

void s2d::UIAnimationEditor::beginWindow()
{
	this->display = true;
	ImGui::Begin("##Editor", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowSize(ImVec2(900, 350));

	ImGui::SetCursorPos(ImVec2(0, 50));
	ImGui::Separator();
}

void s2d::UIAnimationEditor::editorTimeLine()
{
	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_PLAY))
	{
		// Play animation
	}
	ImGui::SameLine();

	std::string text = "    0   ";

	for (int i = 1; i < m_keyFramesToEdit + this->anim->getFrameSize() - 1; i++)
	{
		text += std::to_string(i) + "   ";
	}
	text += std::to_string(m_keyFramesToEdit + this->anim->getFrameSize() - 1);
	ImGui::Text(text.c_str());
}

void s2d::UIAnimationEditor::addKeyFrame()
{
	ImGui::Dummy(ImVec2(0, 200));
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 2 + ImGui::GetScrollX(), ImGui::GetCursorPosY()));
	if (ImGui::Button("Add KeyFrame"))
	{
		if (s2d::UIHirachy::selectedSprite != nullptr)
			this->editor.keyFramePath = s2d::UIHirachy::selectedSprite->path;
		this->editor.isKeyFrameMenuOpen = true;
	}
	if (this->editor.isKeyFrameMenuOpen)
	{
		this->editor.addKeyFrame();
		this->isHovered = editor.isHovered;
	}
	else
	{
		this->isHovered = false;
	}
}

// Public methods

void s2d::UIAnimationEditor::displayEditor()
{
	this->beginWindow();
	this->editorTimeLine();
	this->addKeyFrame();
	this->closeWindow();
}

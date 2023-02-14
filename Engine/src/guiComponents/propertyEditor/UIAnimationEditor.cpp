#include "UIAnimationEditor.h"

//Constructor

s2d::UIAnimationEditor::UIAnimationEditor()
{
	// 100 by default, will increase with the animation size
	this->m_keyFramesToEdit = 100;
	this->editor = s2d::UIAnimationKeyFrameEditor();
	this->m_anim = nullptr;
	this->display = false;
	this->isHovered = false;
}

//Private methods

void s2d::UIAnimationEditor::setAnim(s2d::Animation* anim)
{
	this->m_anim = anim;
}

void s2d::UIAnimationEditor::resetAnim()
{
	this->display = false;
	this->m_anim = nullptr;
}

void s2d::UIAnimationEditor::closeWindow()
{
	std::string name = "Editor - " + this->m_anim->name;

	ImGui::SetCursorPos(ImVec2(8 + ImGui::GetScrollX(), 15));
	ImGui::Text(name.c_str());

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 850 + ImGui::GetScrollX(), ImGui::GetCursorPosY() - 38));

	//Close button
	if (ImGui::Button("x"))
	{
		this->resetAnim();
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
	float y = ImGui::GetCursorPosY() + 10;

	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_PLAY))
	{
		// Play animation
	}

	ImGui::SetCursorPos(ImVec2(100, y));

	for (int i = 1; i <= this->m_keyFramesToEdit; i++)
	{
		ImGui::Text(std::to_string(i).c_str());

		ImGui::SetCursorPos(ImVec2(100 + i * this->m_CURSOR_SPACE, y));
	}
	

	this->renderKeyFrames();
}

void s2d::UIAnimationEditor::renderKeyFrames()
{
	const std::vector<s2d::KeyFrame>& frames = this->m_anim->getKeyFrames();
	const float y = ImGui::GetCursorPosY() + 40;
	float currentMs = 0;

	int cnt = 0;
	std::string keyFrameTimeLine = std::string(10, ' ');
	for (int i = 0; i < this->m_keyFramesToEdit; i++)
	{
		if (cnt == frames.size())
		{
			break;
		}
		if (currentMs == frames[cnt].delay)
		{
			currentMs = 0;
			cnt++;
			ImGui::Text("o");
		}
		currentMs++;

		ImGui::SetCursorPos(ImVec2(100 + i * this->m_CURSOR_SPACE, y));

	}
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

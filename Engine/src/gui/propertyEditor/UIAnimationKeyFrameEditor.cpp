#include "UIAnimationKeyFrameEditor.h"

// Constructor

s2d::UIAnimationKeyFrameEditor::UIAnimationKeyFrameEditor()
{
	this->isHovered = false;
	this->keyFramePath = s2d::SpriteData::defaultSpritePath;
	this->isKeyFrameMenuOpen = false;
}


// Private methods

void s2d::UIAnimationKeyFrameEditor::beginWindow()
{
	ImGui::Begin("##KeyFrameEditor", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowSize(ImVec2(350, 200));
	ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize - 0.25f);

	ImGui::Text("Keyframe Adder");
	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 135, ImGui::GetCursorPosY() - 10));

	//Close button -> it will destroy it (no new keyframe is added) 
	if (ImGui::Button("x"))
	{
		this->isKeyFrameMenuOpen = false;
	}
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

}

void s2d::UIAnimationKeyFrameEditor::inputData()
{
	static int pos = 0;

	ImGui::Text("Position");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(50);
	ImGui::InputInt("##Position", &pos, 0, 0);

	ImGui::Dummy(ImVec2(0, 15));


	ImGui::Text("Sprite");
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 107, ImGui::GetCursorPosY() - 30));
	ImGui::InputText("##spriteAdder", this->keyFramePathArr, CHAR_MAX);

	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(0) && s2d::UIAssetFolder::dragAndDropPath != " ")
	{
		this->keyFramePath = s2d::UIAssetFolder::dragAndDropPath;
		strcpy_s(this->keyFramePathArr, s2d::UIInspector::getNamePathSplit(s2d::UIAssetFolder::dragAndDropPath));
	}
	else
	{
		strcpy_s(this->keyFramePathArr, s2d::UIInspector::getNamePathSplit(this->keyFramePath.c_str()));
	}

}

void s2d::UIAnimationKeyFrameEditor::closeWindowAndSafeKeyFrame()
{
	this->isHovered = ImGui::IsWindowHovered();

	if (ImGui::Button("Done"))
	{
		this->isKeyFrameMenuOpen = false;
	}

	ImGui::End();
}

// Public functions

void s2d::UIAnimationKeyFrameEditor::addKeyFrame()
{
	this->beginWindow();
	this->inputData();
	this->closeWindowAndSafeKeyFrame();
}

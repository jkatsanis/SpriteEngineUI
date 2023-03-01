#include "UIAnimationKeyFrameEditor.h"

// Constructor

s2d::UIAnimationKeyFrameEditor::UIAnimationKeyFrameEditor()
{
	this->pos = 0;
	this->isHovered = false;
	this->keyFramePath = s2d::SpriteData::defaultSpritePath;
	this->isKeyFrameMenuOpen = false;
}


// Private methods

void s2d::UIAnimationKeyFrameEditor::beginWindow()
{
	ImGui::Begin("##KeyFrameEditor", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowSize(ImVec2(350, 200));

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
	ImGui::Text("Position");
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 107, ImGui::GetCursorPosY() - 30));
	ImGui::SetNextItemWidth(50);
	ImGui::InputInt("##Position", &pos, 0, 0);

	ImGui::Dummy(ImVec2(0, 7));


	ImGui::Text("Sprite");
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 107, ImGui::GetCursorPosY() - 30));
	ImGui::InputText("##spriteAdder", this->keyFramePathArr, CHAR_MAX);

	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(0) && s2d::UIAssetFolder::dragAndDropPath != " ")
	{
		this->keyFramePath = s2d::UIAssetFolder::dragAndDropPath;
		strcpy_s(this->keyFramePathArr, s2d::UIInspector::getNamePathSplit(s2d::UIAssetFolder::dragAndDropPath).c_str());
	}
	else
	{
		strcpy_s(this->keyFramePathArr, s2d::UIInspector::getNamePathSplit(this->keyFramePath.c_str()).c_str());
	}

}

void s2d::UIAnimationKeyFrameEditor::closeWindowAndSafeKeyFrame()
{
	this->isHovered = ImGui::IsWindowHovered();

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8);
	if (ImGui::Button("Done"))
	{
		this->addKeyFrameToAnimation();
		this->pos = -1;
		this->keyFramePath = "";
		this->m_animation = nullptr;
		this->isKeyFrameMenuOpen = false;
	}

	ImGui::End();
}

void s2d::UIAnimationKeyFrameEditor::addKeyFrameToAnimation()
{
	if (pos < 0)
	{
		std::cout << "LOG: [ERROR] Tried to add a keyframe at a invalid position!";
	}
	int delay = 0;
	s2d::KeyFrame* changeAfter = nullptr;
	std::vector<s2d::KeyFrame>& ref = this->m_animation->getKeyFrames();
	int size = ref.size();
	int vecpos = 0;
	bool invalid = false;

	for (int i = 0; i < size; i++)
	{
		if (delay < pos)
		{
			delay += ref[i].delay;
			vecpos++;
		}
	}

	vecpos--;

	if (pos == delay)
	{
		std::cout << "LOG: [ERROR] Tried to add a keyframe at a position where already a keyframe exists!";
		vecpos *= -1;
		return;
	}
	if (pos > delay)
	{
		invalid = true;
		vecpos++;
	}
	
	int fndelay = 0;
	if (vecpos > 0 && pos < delay)
	{		
		fndelay = pos - ref[vecpos - 1].position;
		if (vecpos == 1)
		{
			fndelay = pos;
		}
	}
	else
	{
		fndelay = pos - delay;
	}

	if (!invalid)
	{
		int rmDelay = ref[vecpos].delay - pos;
		ref[vecpos].delay = (pos - delay) * -1;
	}
	s2d::KeyFrame add = s2d::KeyFrame(this->keyFramePath, fndelay);
	add.position = pos;

	this->m_animation->addKeyFrameAt(vecpos, add);
}

// Public functions

void s2d::UIAnimationKeyFrameEditor::update()
{
	this->beginWindow();
	this->inputData();
	this->closeWindowAndSafeKeyFrame();
}

void s2d::UIAnimationKeyFrameEditor::setAnimation(s2d::Animation* anim)
{
	this->m_animation = anim;
}

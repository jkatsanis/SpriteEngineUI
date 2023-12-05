#include "UIAnimationKeyFrameAdder.h"

// Constructor

void spe::UIAnimationKeyFrameAdder::Init()
{
	this->m_KeyFramePos = 0;
	this->Hovered = false;
	this->IsKeyFrameMenuOpen = false;
	this->KeyFramePath = PATH_TO_DEFAULT_SPRITE;
}


// Private methods

void spe::UIAnimationKeyFrameAdder::BeginWindow()
{
	ImGui::Begin("##KeyFrameEditor", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowSize(ImVec2(350, 200));

	ImGui::Text("Keyframe Adder");
	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 135, ImGui::GetCursorPosY() - 10));

	//Close button -> it will destroy it (no new keyframe is added) 
	if (ImGui::Button("x"))
	{
		this->IsKeyFrameMenuOpen = false;
	}
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

}

void spe::UIAnimationKeyFrameAdder::InputData()
{
	ImGui::Text("Position");
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 107, ImGui::GetCursorPosY() - 30));
	ImGui::SetNextItemWidth(50);
	ImGui::InputInt("##Position", &m_KeyFramePos, 0, 0);

	ImGui::Dummy(ImVec2(0, 7));


	ImGui::Text("Sprite");
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 107, ImGui::GetCursorPosY() - 30));
	ImGui::InputText("##spriteAdder", this->m_KeyFramePath, CHAR_MAX);

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped) && ImGui::IsMouseReleased(0) && this->m_ptr_GUIRepo->DragAndDropPath != " ")
	{
		this->KeyFramePath = this->m_ptr_GUIRepo->DragAndDropPath;
		strcpy_s(this->m_KeyFramePath, spe::Utility::GetNamePathSplit(this->m_ptr_GUIRepo->DragAndDropPath).c_str());
	}
	else
	{
		strcpy_s(this->m_KeyFramePath, spe::Utility::GetNamePathSplit(this->KeyFramePath.c_str()).c_str());
	}
}

void spe::UIAnimationKeyFrameAdder::CloseWindowAndSafeKeyFrame()
{
	this->Hovered = spe::UIUtility::IsHovered(ImGui::GetWindowSize(), ImGui::GetWindowPos());

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8);
	if (ImGui::Button("Done"))
	{
		this->AddKeyFrameToAnimation();
		this->Reset();
	}

	ImGui::End();
}

void spe::UIAnimationKeyFrameAdder::AddKeyFrameToAnimation()
{
	if (this->m_KeyFramePos < 0)
	{
		std::cout << "LOG: [ERROR] Tried to add a keyframe at a invalid position!";
		return;
	}
	if (spe::Utility::GetFileExtension(this->KeyFramePath) != "png")
	{
		std::cout << "LOG: [ERROR] Tried to add a invalid file type!";
		return;
	}
	
	float delay = 0;
	std::vector<spe::KeyFrame>& ref = this->m_ptr_Anim->GetkeyFrames();
	int size = (int)ref.size();
	int vecpos = 0;
	bool invalid = false;

	for (int i = 0; i < size; i++)
	{
		if (delay < this->m_KeyFramePos)
		{
			delay += ref[i].delay;
			vecpos++;
		}
	}

	vecpos--;

	if (this->m_KeyFramePos == 0 && ref.size() == 0
		|| this->m_KeyFramePos == 0 && ref.size() != 0 && ref[0].position != 0)
	{
		spe::KeyFrame add = spe::KeyFrame(this->KeyFramePath, 0);
		add.position = this->m_KeyFramePos;

		this->m_ptr_Anim->AddKeyFrameAt(0, add);
		return;
	}
	

	if (this->m_KeyFramePos == delay)
	{
		std::cout << "LOG: [ERROR] Tried to add a keyframe at a position where already a keyframe exists!";
		vecpos *= -1;
		return;
	}
	if (this->m_KeyFramePos > delay)
	{
		invalid = true;
		vecpos++;
	}
	
	float fndelay = 0;
	if (vecpos > 0 && this->m_KeyFramePos < delay)
	{		
		int before = vecpos - 1;
		fndelay = (float)this->m_KeyFramePos - ref[before].position;

		if (vecpos == 1)
		{
			fndelay = (float)this->m_KeyFramePos;
		}
	}
	else
	{
		fndelay = this->m_KeyFramePos - delay;
	}

	if (!invalid)
	{
		ref[vecpos].delay = (this->m_KeyFramePos - delay) * -1;
	}

	spe::KeyFrame add = spe::KeyFrame(this->KeyFramePath, fndelay);
	add.position = this->m_KeyFramePos;

	this->m_ptr_Anim->AddKeyFrameAt(vecpos, add);

	if (ref[0].delay < 0)
	{
		ref[0].delay = (float)ref[0].position;
	}
}


// Public functions

void spe::UIAnimationKeyFrameAdder::Render()
{
	this->BeginWindow();
	this->InputData();
	this->CloseWindowAndSafeKeyFrame();
}

void spe::UIAnimationKeyFrameAdder::SetAnimation(spe::Animation* anim)
{
	this->m_ptr_Anim = anim;
}

void spe::UIAnimationKeyFrameAdder::Reset()
{
	this->m_KeyFramePos = -1;
	this->KeyFramePath = "";
	this->m_ptr_Anim = nullptr;
	this->IsKeyFrameMenuOpen = false;
	this->Hovered = false;
}

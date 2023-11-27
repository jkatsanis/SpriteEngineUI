#include "UIAnimationKeyFrameAdder.h"

// Constructor

void spe::UIAnimationKeyFrameAdder::Init()
{
	this->m_key_frame_pos = 0;
	this->is_hovered = false;
	this->is_key_frame_menu_open = false;
	this->key_frame_path = PATH_TO_DEFAULT_SPRITE;
}


// Private methods

void spe::UIAnimationKeyFrameAdder::beginWindow()
{
	ImGui::Begin("##KeyFrameEditor", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowSize(ImVec2(350, 200));

	ImGui::Text("Keyframe Adder");
	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 135, ImGui::GetCursorPosY() - 10));

	//Close button -> it will destroy it (no new keyframe is added) 
	if (ImGui::Button("x"))
	{
		this->is_key_frame_menu_open = false;
	}
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

}

void spe::UIAnimationKeyFrameAdder::inputData()
{
	ImGui::Text("Position");
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 107, ImGui::GetCursorPosY() - 30));
	ImGui::SetNextItemWidth(50);
	ImGui::InputInt("##Position", &m_key_frame_pos, 0, 0);

	ImGui::Dummy(ImVec2(0, 7));


	ImGui::Text("Sprite");
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 107, ImGui::GetCursorPosY() - 30));
	ImGui::InputText("##spriteAdder", this->m_keyFramePathDataHolder, CHAR_MAX);

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped) && ImGui::IsMouseReleased(0) && this->m_ptr_GUIRepo->DragAndDropPath != " ")
	{
		this->key_frame_path = this->m_ptr_GUIRepo->DragAndDropPath;
		strcpy_s(this->m_keyFramePathDataHolder, spe::Utility::GetNamePathSplit(this->m_ptr_GUIRepo->DragAndDropPath).c_str());
	}
	else
	{
		strcpy_s(this->m_keyFramePathDataHolder, spe::Utility::GetNamePathSplit(this->key_frame_path.c_str()).c_str());
	}
}

void spe::UIAnimationKeyFrameAdder::closeWindowAndSafeKeyFrame()
{
	this->is_hovered = spe::UIUtility::IsHovered(ImGui::GetWindowSize(), ImGui::GetWindowPos());

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8);
	if (ImGui::Button("Done"))
	{
		this->addKeyFrameToAnimation();
		this->reset();
	}

	ImGui::End();
}

void spe::UIAnimationKeyFrameAdder::addKeyFrameToAnimation()
{
	if (this->m_key_frame_pos < 0)
	{
		std::cout << "LOG: [ERROR] Tried to add a keyframe at a invalid position!";
		return;
	}
	if (spe::Utility::GetFileExtension(this->key_frame_path) != "png")
	{
		std::cout << "LOG: [ERROR] Tried to add a invalid file type!";
		return;
	}
	
	float delay = 0;
	spe::KeyFrame* changeAfter = nullptr;
	std::vector<spe::KeyFrame>& ref = this->m_animation->GetkeyFrames();
	int size = (int)ref.size();
	int vecpos = 0;
	bool invalid = false;

	for (int i = 0; i < size; i++)
	{
		if (delay < this->m_key_frame_pos)
		{
			delay += ref[i].delay;
			vecpos++;
		}
	}

	vecpos--;

	if (this->m_key_frame_pos == 0 && ref.size() == 0
		|| this->m_key_frame_pos == 0 && ref.size() != 0 && ref[0].position != 0)
	{
		spe::KeyFrame add = spe::KeyFrame(this->key_frame_path, 0);
		add.position = this->m_key_frame_pos;

		this->m_animation->AddKeyFrameAt(0, add);
		return;
	}
	

	if (this->m_key_frame_pos == delay)
	{
		std::cout << "LOG: [ERROR] Tried to add a keyframe at a position where already a keyframe exists!";
		vecpos *= -1;
		return;
	}
	if (this->m_key_frame_pos > delay)
	{
		invalid = true;
		vecpos++;
	}
	
	float fndelay = 0;
	if (vecpos > 0 && this->m_key_frame_pos < delay)
	{		
		int before = vecpos - 1;
		fndelay = (float)this->m_key_frame_pos - ref[before].position;

		if (vecpos == 1)
		{
			fndelay = (float)this->m_key_frame_pos;
		}
	}
	else
	{
		fndelay = this->m_key_frame_pos - delay;
	}

	if (!invalid)
	{
		float rmDelay = ref[vecpos].delay - this->m_key_frame_pos;
		ref[vecpos].delay = (this->m_key_frame_pos - delay) * -1;
	}

	spe::KeyFrame add = spe::KeyFrame(this->key_frame_path, fndelay);
	add.position = this->m_key_frame_pos;

	this->m_animation->AddKeyFrameAt(vecpos, add);

	if (ref[0].delay < 0)
	{
		ref[0].delay = (float)ref[0].position;
	}
}


// Public functions

void spe::UIAnimationKeyFrameAdder::Render()
{
	this->beginWindow();
	this->inputData();
	this->closeWindowAndSafeKeyFrame();
}

void spe::UIAnimationKeyFrameAdder::setAnimation(spe::Animation* anim)
{
	this->m_animation = anim;
}

void spe::UIAnimationKeyFrameAdder::reset()
{
	this->m_key_frame_pos = -1;
	this->key_frame_path = "";
	this->m_animation = nullptr;
	this->is_key_frame_menu_open = false;
	this->is_hovered = false;
}

#include "UIAnimationKeyFrameAdder.h"

// Constructor

s2d::UIAnimationKeyFrameAdder::UIAnimationKeyFrameAdder()
{
	this->m_key_frame_pos = 0;
	this->isHovered = false;
	this->keyFramePath = s2d::SpriteData::s_default_sprite_path;
	this->isKeyFrameMenuOpen = false;
}


// Private methods

void s2d::UIAnimationKeyFrameAdder::beginWindow()
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

void s2d::UIAnimationKeyFrameAdder::inputData()
{
	ImGui::Text("Position");
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 107, ImGui::GetCursorPosY() - 30));
	ImGui::SetNextItemWidth(50);
	ImGui::InputInt("##Position", &m_key_frame_pos, 0, 0);

	ImGui::Dummy(ImVec2(0, 7));


	ImGui::Text("Sprite");
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 107, ImGui::GetCursorPosY() - 30));
	ImGui::InputText("##spriteAdder", this->m_keyFramePathDataHolder, CHAR_MAX);

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped) && ImGui::IsMouseReleased(0) && this->m_ptr_repo->asset_folder_data.drag_and_drop_path != " ")
	{
		this->keyFramePath = this->m_ptr_repo->asset_folder_data.drag_and_drop_path;
		strcpy_s(this->m_keyFramePathDataHolder, s2d::UIInspector::getNamePathSplit(this->m_ptr_repo->asset_folder_data.drag_and_drop_path).c_str());
	}
	else
	{
		strcpy_s(this->m_keyFramePathDataHolder, s2d::UIInspector::getNamePathSplit(this->keyFramePath.c_str()).c_str());
	}
}

void s2d::UIAnimationKeyFrameAdder::closeWindowAndSafeKeyFrame()
{
	this->isHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenBlockedByPopup);

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8);
	if (ImGui::Button("Done"))
	{
		this->addKeyFrameToAnimation();
		this->reset();
	}

	ImGui::End();
}

void s2d::UIAnimationKeyFrameAdder::addKeyFrameToAnimation()
{
	if (this->m_key_frame_pos < 0)
	{
		std::cout << "LOG: [ERROR] Tried to add a keyframe at a invalid position!";
		return;
	}
	if (std::getFileExtension(this->keyFramePath) != "png")
	{
		std::cout << "LOG: [ERROR] Tried to add a invalid file type!";
		return;
	}
	
	int delay = 0;
	s2d::KeyFrame* changeAfter = nullptr;
	std::vector<s2d::KeyFrame>& ref = this->m_animation->getKeyFrames();
	int size = (int)ref.size();
	int vecpos = 0;
	bool invalid = false;

	for (int i = 0; i < size; i++)
	{
		if (delay < m_key_frame_pos)
		{
			delay += ref[i].delay;
			vecpos++;
		}
	}

	vecpos--;

	if (this->m_key_frame_pos == 0 && ref.size() == 0
		|| this->m_key_frame_pos == 0 && ref.size() != 0 && ref[0].position != 0)
	{
		s2d::KeyFrame add = s2d::KeyFrame(this->keyFramePath, 0);
		add.position = this->m_key_frame_pos;

		this->m_animation->addKeyFrameAt(0, add);
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
	
	int fndelay = 0;
	if (vecpos > 0 && this->m_key_frame_pos < delay)
	{		
		int before = vecpos - 1;
		fndelay = this->m_key_frame_pos - ref[before].position;

		if (vecpos == 1)
		{
			fndelay = this->m_key_frame_pos;
		}
	}
	else
	{
		fndelay = this->m_key_frame_pos - delay;
	}

	if (!invalid)
	{
		int rmDelay = ref[vecpos].delay - this->m_key_frame_pos;
		ref[vecpos].delay = (this->m_key_frame_pos - delay) * -1;
	}

	s2d::KeyFrame add = s2d::KeyFrame(this->keyFramePath, fndelay);
	add.position = this->m_key_frame_pos;

	this->m_animation->addKeyFrameAt(vecpos, add);

	if (ref[0].delay < 0)
	{
		ref[0].delay = ref[0].position;
	}
}

// Public functions

void s2d::UIAnimationKeyFrameAdder::update()
{
	this->beginWindow();
	this->inputData();
	this->closeWindowAndSafeKeyFrame();
}

void s2d::UIAnimationKeyFrameAdder::setAnimation(s2d::Animation* anim)
{
	this->m_animation = anim;
}

void s2d::UIAnimationKeyFrameAdder::reset()
{
	this->m_key_frame_pos = -1;
	this->keyFramePath = "";
	this->m_animation = nullptr;
	this->isKeyFrameMenuOpen = false;
	this->isHovered = false;
}

#include "UIAnimationEditor.h"

//Constructor

s2d::UIAnimationEditor::UIAnimationEditor()
{
	// 100 by default, will increase with the animation size
	this->m_keyFramesToEdit = 100;
	this->keyFrameAdder = s2d::UIAnimationKeyFrameAdder();
	this->m_anim = nullptr;
	this->display = false;
	this->m_keyFrameSelected.keyFrameSelected = nullptr;
	this->is_hovered = false;
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

	if (!this->is_hovered)
		this->is_hovered = ImGui::IsWindowHovered();

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

	if (!this->m_anim->is_playing)
	{
		if (s2d::FontManager::displaySmybolAsButton(ICON_FA_PLAY))
		{
			this->m_anim->play();
		}
	}
	else
	{
		if (s2d::FontManager::displaySmybolAsButton(ICON_FA_SQUARE))
		{
			this->m_anim->stop();
		}
	}

	ImGui::SetCursorPos(ImVec2(100, y));

	for (int i = 0; i <= this->m_keyFramesToEdit; i++)
	{
		ImGui::Text(std::to_string(i).c_str());

		ImGui::SetCursorPos(ImVec2(100.0f + ((int)i + 1.0f) * CURSOR_SPACE, y));
	}


	this->renderKeyFrames();
	this->displayKeyFrameInfo();
}

void s2d::UIAnimationEditor::displayKeyFrameInfo()
{
	if (this->m_keyFrameSelected.keyFrameSelected == nullptr)
	{
		this->m_keyFrameSelected.position = -1;
		return;
	}

	const std::string keyFrameSelected = "KeyFrame Selected: " + std::to_string(this->m_keyFrameSelected.position);
	ImGui::Text(keyFrameSelected.c_str());

	if (ImGui::Button("Delete"))
	{
		this->m_anim->play();
		this->m_anim->stop();
		this->m_anim->deleteKeyFrame(this->m_keyFrameSelected.position);
		this->m_keyFrameSelected.keyFrameSelected = nullptr;
	}
}

void s2d::UIAnimationEditor::renderKeyFrames()
{
	const static float s_sizeTo0Frame = 100;
	const static float s_keyFrameSize = 18;
	const std::vector<s2d::KeyFrame>& frames = this->m_anim->getKeyFrames();
	const float y = ImGui::GetCursorPosY() + 40;
	float currentMs = 0;
	float add = 0;
	int cnt = 0;
	std::string keyFrameTimeLine = std::string(10, ' ');

	ImGui::NewLine();
	ImGui::SetCursorPos(ImVec2(s_sizeTo0Frame , y));

	for (int i = 0; i < this->m_keyFramesToEdit; i++)
	{
		if (cnt == frames.size())
		{
			break;
		}
		if (currentMs == frames[cnt].delay)
		{
			if (i >= 10)
			{
				add = 6;
			}

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - s_keyFrameSize / 2 + add);
			ImGui::PushStyleColor(ImGuiCol_Button, REAL_EDITOR_BUTTON_BG_COLOR);

			const std::string buttonName = "o" + std::string("##") + std::to_string(i);

			if (ImGui::Button(buttonName.c_str()))
			{
				this->m_keyFrameSelected.isClicked = true;
				this->m_keyFrameSelected.keyFrameSelected = &frames[cnt];
				this->m_keyFrameSelected.position = i;
			}
			ImGui::PopStyleColor();

			currentMs = 0;
			cnt++;
		}
		currentMs++;

		ImGui::SetCursorPos(ImVec2(100.0f + ((int)i + 1.0f) * CURSOR_SPACE, y));
	}
	ImGui::NewLine();
	ImGui::Dummy(ImVec2(0, 20));
}

void s2d::UIAnimationEditor::addKeyFrame()
{
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 2 + ImGui::GetScrollX(), ImGui::GetCursorPosY()));
	if (ImGui::Button("Add KeyFrame"))
	{
		if (this->m_ptr_repo->sprite_in_inspector != nullptr)
		{
			this->keyFrameAdder.keyFramePath = this->m_ptr_repo->sprite_in_inspector->sprite_renderer.path;
		}
		this->keyFrameAdder.isKeyFrameMenuOpen = true;
		this->keyFrameAdder.setAnimation(this->m_anim);
	}
	this->is_hovered = (this->keyFrameAdder.isHovered) 
		? true 
		: ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenBlockedByPopup);
	
}

// Public methods

void s2d::UIAnimationEditor::displayEditor()
{
	this->beginWindow();
	this->editorTimeLine();
	this->addKeyFrame();
	this->closeWindow(); 

	if (this->keyFrameAdder.isKeyFrameMenuOpen)
	{
		this->keyFrameAdder.update();
	}
}

void s2d::UIAnimationEditor::setSpriteRepository(s2d::SpriteRepository& repo)
{
	this->m_ptr_repo = &repo;
	this->keyFrameAdder.setSpriteRepository(repo);
}

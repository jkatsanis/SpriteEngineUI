#include "UIAnimationEditor.h"

//Constructor
//Private methods

void spe::UIAnimationEditor::Init()
{
	this->m_keyFramesToEdit = 10000;
	this->m_anim = nullptr;
	this->display = false;
	this->m_keyFrameSelected.keyFrameSelected = nullptr;
	this->m_cursor_space = 30;

	this->m_FrameAdder.SetRepos(this->m_ptr_Repo, this->m_ptr_GUIRepo);
}

void spe::UIAnimationEditor::setAnim(spe::Animation* anim)
{
	this->m_anim = anim;
}

void spe::UIAnimationEditor::resetAnim()
{
	this->display = false;
	this->m_anim = nullptr;
}

void spe::UIAnimationEditor::closeWindow()
{
	std::string name = "Editor - " + this->m_anim->GetName();

	ImGui::SetCursorPos(ImVec2(8 + ImGui::GetScrollX(), 15));
	ImGui::Text(name.c_str());

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 850 + ImGui::GetScrollX(), ImGui::GetCursorPosY() - 38));

	//Close button
	if (ImGui::Button("x"))
	{
		this->m_anim->ptr_AppliedSprite->animator.stop(this->m_anim->GetName());
		this->resetAnim();
	}

    this->Hovered = this->m_FrameAdder.Hovered;
	if (!this->Hovered)
	{
		this->Hovered = spe::UIUtility::IsHovered(ImGui::GetWindowPos(), WINDOW_SIZE_ANIMATION_EDITOR);
	}


	ImGui::End();
}

void spe::UIAnimationEditor::beginWindow()
{
	this->display = true;
	ImGui::Begin("##Editor", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowSize(WINDOW_SIZE_ANIMATION_EDITOR);

	ImGui::SetCursorPos(ImVec2(0, 50));
	ImGui::Separator();
}

void spe::UIAnimationEditor::editorTimeLine()
{
	float y = ImGui::GetCursorPosY() + 10;

	if (!this->m_anim->IsPlaying)
	{
		if (spe::Style::DisplaySmybolAsButton(ICON_FA_PLAY))
		{
			this->m_anim->ptr_AppliedSprite->animator.play(this->m_anim->GetName());
		}
	}
	else
	{
		if (spe::Style::DisplaySmybolAsButton(ICON_FA_SQUARE))
		{
			this->m_anim->ptr_AppliedSprite->animator.stop(this->m_anim->GetName());
		}
	}

	ImGui::SetCursorPos(ImVec2(100, y));

	for (int i = 0; i <= this->m_keyFramesToEdit; i++)
	{
		if (this->displayTimeFrameBasedOnCursorSpace(i) && this->renderTextBasedOnScroll(i))
		{
			ImGui::Text(std::to_string(i).c_str());
		}
		ImGui::SetCursorPos(ImVec2(100.0f + ((int)i + 1.0f) * this->m_cursor_space, y));
	}

	this->renderKeyFrames();
	this->displayKeyFrameInfo();
}

bool spe::UIAnimationEditor::displayTimeFrameBasedOnCursorSpace(size_t i_pos)
{
	if (this->m_cursor_space >= 25)
	{
		return true;
	}

	if (i_pos % 5 == 0 && this->m_cursor_space <= 24 && this->m_cursor_space > MAX_CURSOR_SPACE)
	{
		return true;
	}
	if (i_pos % 10 == 0 && this->m_cursor_space <= MAX_CURSOR_SPACE && this->m_cursor_space > MAX_CURSOR_SPACE - SMALL_INCREMENT)
	{
		return true;
	}
	if (i_pos % 20 == 0 && this->m_cursor_space <= MAX_CURSOR_SPACE - SMALL_INCREMENT && this->m_cursor_space > MIN_CURSOR_SPACE)
	{
		return true;
	}
	if (i_pos % 50 == 0 && this->m_cursor_space <= MIN_CURSOR_SPACE && this->m_cursor_space > MIN_CURSOR_SPACE - SMALL_INCREMENT)
	{
		return true;
	}
	if (i_pos % 100 == 0 && this->m_cursor_space <= MIN_CURSOR_SPACE - SMALL_INCREMENT)
	{
		return true;
	}
	return false;
}

void spe::UIAnimationEditor::displayKeyFrameInfo()
{
	if (this->m_keyFrameSelected.keyFrameSelected == nullptr)
	{
		this->m_keyFrameSelected.position = -1;
		return;
	}

	const ImVec2 pos = ImGui::GetCursorPos();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250);

	const std::string keyFrameSelected = "KeyFrame Selected: " + std::to_string(this->m_keyFrameSelected.position);
	ImGui::Text(keyFrameSelected.c_str());

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 245);

	if (ImGui::Button("Delete"))
	{
		this->m_anim->Stop();
		this->m_anim->DeleteKeyFrame(this->m_keyFrameSelected.position);
		this->m_keyFrameSelected.keyFrameSelected = nullptr;
	}
	ImGui::SameLine();
	spe::Style::DisplaySmybolAsText(ICON_FA_TRASH);


	ImGui::SetCursorPos(pos);
}

bool spe::UIAnimationEditor::renderTextBasedOnScroll(size_t i)
{
	float minus = 50;
	if (this->m_cursor_space > 20)
	{
		minus = 50;
	}
	else if (this->m_cursor_space > 10)
	{
		minus = 100;
	}
	else if(this->m_cursor_space > 5)
	{
		minus = 1000;
	}
	else
	{
		minus = 2000;
	}

	const float max = ImGui::GetScrollMaxX();
    float scroll = ImGui::GetScrollX();
	float divisor = max / this->m_keyFramesToEdit;

	scroll /= divisor;

	return i > scroll - minus && i < scroll + minus;
}

void spe::UIAnimationEditor::renderTimeLineRealTimePoint()
{
	const ImVec2 cursor_pos = ImGui::GetCursorPos();
	ImGui::SetCursorPosX(100);
	const float y = ImGui::GetCursorPosY() + 5;
	bool b = false;
	for (float i = 0; i < this->m_anim->TotalTimePassed * 100; i += 0.5f)
	{	
		ImGui::SetCursorPos(ImVec2(100 + ((int)i + 1.0f) * this->m_cursor_space, y));
	}
	ImGui::Text("|");

	ImGui::SetCursorPos(cursor_pos);
}

void spe::UIAnimationEditor::zoomEditorTimeLine()
{
	if (this->m_ptr_GUIRepo->ptr_SFEvent->type == sf::Event::MouseWheelScrolled)
	{
		this->m_ptr_GUIRepo->ptr_SFEvent->type = sf::Event::GainedFocus;

		if (this->m_ptr_GUIRepo->ptr_SFEvent->mouseWheel.x < 0)
		{
			if (this->m_cursor_space == 0.5f)
			{
				return;
			}
			if (this->m_cursor_space <= MIN_CURSOR_SPACE)
			{
				this->m_cursor_space -= SMALL_INCREMENT;
			}
			else if (this->m_cursor_space <= MAX_CURSOR_SPACE)
			{
				this->m_cursor_space -= LARGE_INCREMENT;
			}
			else
			{
				this->m_cursor_space -= MAX_CURSOR_SPACE;
			}
		}
		else
		{
			if (this->m_cursor_space < MIN_CURSOR_SPACE)
			{
				this->m_cursor_space += SMALL_INCREMENT;
			}
			else if (this->m_cursor_space < MAX_CURSOR_SPACE)
			{
				this->m_cursor_space += LARGE_INCREMENT;
			}
			else
			{
				this->m_cursor_space += MAX_CURSOR_SPACE;
			}
		}
	}
}

void spe::UIAnimationEditor::renderKeyFrames()
{
	const static float s_sizeTo0Frame = 100;
	const static float s_keyFrameSize = 18;
	const std::vector<spe::KeyFrame>& frames = this->m_anim->GetkeyFrames();
	const float y = ImGui::GetCursorPosY() + 40;
	float currentMs = 0;
	float add = 0;
	int cnt = 0;
	std::string keyFrameTimeLine = std::string(10, ' ');

	ImGui::NewLine();
	ImGui::SetCursorPos(ImVec2(s_sizeTo0Frame , y));

	for (int i = 0; i <= this->m_anim->GetAnimationTime(); i++)
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

		ImGui::SetCursorPos(ImVec2(100.0f + ((int)i + 1.0f) * this->m_cursor_space, y));
	}
	ImGui::NewLine();
	ImGui::Dummy(ImVec2(0, 20));
}

void spe::UIAnimationEditor::addKeyFrame()
{
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 2 + ImGui::GetScrollX(), ImGui::GetCursorPosY()));
	if (ImGui::Button("Add KeyFrame"))
	{
		if (this->m_ptr_GUIRepo->sprite_in_inspector != nullptr)
		{
			this->m_FrameAdder.key_frame_path = this->m_ptr_GUIRepo->sprite_in_inspector->sprite_renderer.path;
		}
		this->m_FrameAdder.is_key_frame_menu_open = true;
		this->m_FrameAdder.setAnimation(this->m_anim);
	}
	ImGui::SameLine();
	spe::Style::DisplaySmybolAsText(ICON_FA_PLUS);
}

void spe::UIAnimationEditor::saveAnimation()
{
	if (ImGui::Button("Save"))
	{
		spe::Savesystem::CreateAnimationSaveFile(this->m_ptr_GUIRepo->sprite_in_inspector, *this->m_anim);
	}
	spe::UIUtility::SameLine(2);
	spe::Style::DisplaySmybolAsText(ICON_FA_SAVE);
}

void spe::UIAnimationEditor::renameAnimation()
{
	static std::string s_renamed_pop_up_name = "";

	if (ImGui::Button("Rename"))
	{
		ImGui::OpenPopup("First");
	}

	if (ImGui::BeginPopup("First"))
	{
		ImGui::Text("Rename    -    Changes will be permanent");
		ImGui::Separator();
		ImGui::InputTextWithHint("##rename-anim", "<name>", &s_renamed_pop_up_name[0], CHAR_MAX);

		spe::UIUtility::SameLine(2);
		
		if (spe::Style::DisplaySmybolAsButton(ICON_FA_ARROW_RIGHT))
		{
			const char* name = s_renamed_pop_up_name.c_str();
			const std::string std_name(name);
			this->m_ptr_GUIRepo->sprite_in_inspector->animator.setName(std_name, this->m_anim->GetName());
			// Getting the new animation becuase the old 1 got deleted
			this->m_anim = &this->m_ptr_GUIRepo->sprite_in_inspector->animator.animations[std_name];
			
			s_renamed_pop_up_name = "";
		}
		
		this->Hovered = spe::UIUtility::IsHovered(ImGui::GetWindowSize(), ImGui::GetWindowPos());

		ImGui::EndPopup();
	}

	spe::UIUtility::SameLine(2);
	spe::Style::DisplaySmybolAsText(ICON_FA_EDIT);
}

// Public methods

void spe::UIAnimationEditor::Render()
{
	if (this->m_ptr_GUIRepo->sprite_in_inspector == nullptr)
	{
		return;
	}
	this->zoomEditorTimeLine();
	this->beginWindow();
	this->renderTimeLineRealTimePoint();
	this->editorTimeLine();
	this->addKeyFrame();

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
	ImGui::Text("Loop");
	ImGui::SameLine();
	ImGui::Checkbox("##loop", &this->m_anim->Loop);

	this->saveAnimation();
	this->renameAnimation();

	this->closeWindow();


	if (this->m_FrameAdder.is_key_frame_menu_open)
	{
		this->m_FrameAdder.Render();
	}
}

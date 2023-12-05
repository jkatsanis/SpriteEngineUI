#include "UIAnimationEditor.h"

//Constructor
//Private methods

void spe::UIAnimationEditor::Init()
{
	this->m_KeyFramesToEdit = 10000;
	this->m_ptr_Anim = nullptr;
	this->Display = false;
	this->m_KeyFrameSelected.keyFrameSelected = nullptr;
	this->m_CursorSpace = 30;

	this->m_FrameAdder.SetRepos(this->m_ptr_Repo, this->m_ptr_GUIRepo);
}

void spe::UIAnimationEditor::SetAnim(spe::Animation* anim)
{
	this->m_ptr_Anim = anim;
}

void spe::UIAnimationEditor::ResetAnim()
{
	this->Display = false;
	this->m_ptr_Anim = nullptr;
}

void spe::UIAnimationEditor::CloseWindow()
{
	std::string name = "Editor - " + this->m_ptr_Anim->GetName();

	ImGui::SetCursorPos(ImVec2(8 + ImGui::GetScrollX(), 15));
	ImGui::Text(name.c_str());

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 850 + ImGui::GetScrollX(), ImGui::GetCursorPosY() - 38));

	//Close button
	if (ImGui::Button("x"))
	{
		this->m_ptr_Anim->ptr_AppliedSprite->Animator.Stop(this->m_ptr_Anim->GetName());
		this->ResetAnim();
	}

    this->Hovered = this->m_FrameAdder.Hovered;
	if (!this->Hovered)
	{
		this->Hovered = spe::UIUtility::IsHovered(ImGui::GetWindowPos(), WINDOW_SIZE_ANIMATION_EDITOR);
	}


	ImGui::End();
}

void spe::UIAnimationEditor::BeginWindow()
{
	this->Display = true;
	ImGui::Begin("##Editor", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowSize(WINDOW_SIZE_ANIMATION_EDITOR);

	ImGui::SetCursorPos(ImVec2(0, 50));
	ImGui::Separator();
}

void spe::UIAnimationEditor::EditorTimeLine()
{
	float y = ImGui::GetCursorPosY() + 10;

	if (!this->m_ptr_Anim->IsPlaying)
	{
		if (spe::Style::DisplaySmybolAsButton(ICON_FA_PLAY))
		{
			this->m_ptr_Anim->ptr_AppliedSprite->Animator.Play(this->m_ptr_Anim->GetName());
		}
	}
	else
	{
		if (spe::Style::DisplaySmybolAsButton(ICON_FA_SQUARE))
		{
			this->m_ptr_Anim->ptr_AppliedSprite->Animator.Stop(this->m_ptr_Anim->GetName());
		}
	}

	ImGui::SetCursorPos(ImVec2(100, y));

	for (int i = 0; i <= this->m_KeyFramesToEdit; i++)
	{
		if (this->DisplayTimeFrameBasedOnCursorSpace(i) && this->RenderTextBasedOnScroll(i))
		{
			ImGui::Text(std::to_string(i).c_str());
		}
		ImGui::SetCursorPos(ImVec2(100.0f + ((int)i + 1.0f) * this->m_CursorSpace, y));
	}

	this->RenderKeyFrames();
	this->DisplayKeyFrameInfo();
}

bool spe::UIAnimationEditor::DisplayTimeFrameBasedOnCursorSpace(size_t i_pos)
{
	if (this->m_CursorSpace >= 25)
	{
		return true;
	}

	if (i_pos % 5 == 0 && this->m_CursorSpace <= 24 && this->m_CursorSpace > MAX_CURSOR_SPACE)
	{
		return true;
	}
	if (i_pos % 10 == 0 && this->m_CursorSpace <= MAX_CURSOR_SPACE && this->m_CursorSpace > MAX_CURSOR_SPACE - SMALL_INCREMENT)
	{
		return true;
	}
	if (i_pos % 20 == 0 && this->m_CursorSpace <= MAX_CURSOR_SPACE - SMALL_INCREMENT && this->m_CursorSpace > MIN_CURSOR_SPACE)
	{
		return true;
	}
	if (i_pos % 50 == 0 && this->m_CursorSpace <= MIN_CURSOR_SPACE && this->m_CursorSpace > MIN_CURSOR_SPACE - SMALL_INCREMENT)
	{
		return true;
	}
	if (i_pos % 100 == 0 && this->m_CursorSpace <= MIN_CURSOR_SPACE - SMALL_INCREMENT)
	{
		return true;
	}
	return false;
}

void spe::UIAnimationEditor::DisplayKeyFrameInfo()
{
	if (this->m_KeyFrameSelected.keyFrameSelected == nullptr)
	{
		this->m_KeyFrameSelected.position = -1;
		return;
	}

	const ImVec2 pos = ImGui::GetCursorPos();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 250);

	const std::string keyFrameSelected = "KeyFrame Selected: " + std::to_string(this->m_KeyFrameSelected.position);
	ImGui::Text(keyFrameSelected.c_str());

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 245);

	if (ImGui::Button("Delete"))
	{
		this->m_ptr_Anim->Stop();
		this->m_ptr_Anim->DeleteKeyFrame(this->m_KeyFrameSelected.position);
		this->m_KeyFrameSelected.keyFrameSelected = nullptr;
	}
	ImGui::SameLine();
	spe::Style::DisplaySmybolAsText(ICON_FA_TRASH);


	ImGui::SetCursorPos(pos);
}

bool spe::UIAnimationEditor::RenderTextBasedOnScroll(size_t i)
{
	float minus = 50;
	if (this->m_CursorSpace > 20)
	{
		minus = 50;
	}
	else if (this->m_CursorSpace > 10)
	{
		minus = 100;
	}
	else if(this->m_CursorSpace > 5)
	{
		minus = 1000;
	}
	else
	{
		minus = 2000;
	}

	const float max = ImGui::GetScrollMaxX();
    float scroll = ImGui::GetScrollX();
	float divisor = max / this->m_KeyFramesToEdit;

	scroll /= divisor;

	return i > scroll - minus && i < scroll + minus;
}

void spe::UIAnimationEditor::RenderTimeLineRealTimePoint()
{
	const ImVec2 cursor_pos = ImGui::GetCursorPos();
	ImGui::SetCursorPosX(100);
	const float y = ImGui::GetCursorPosY() + 5;
	for (float i = 0; i < this->m_ptr_Anim->TotalTimePassed * 100; i += 0.5f)
	{	
		ImGui::SetCursorPos(ImVec2(100 + ((int)i + 1.0f) * this->m_CursorSpace, y));
	}
	ImGui::Text("|");

	ImGui::SetCursorPos(cursor_pos);
}

void spe::UIAnimationEditor::ZoomEditorTimeLine()
{
	if (this->m_ptr_GUIRepo->ptr_SFEvent->type == sf::Event::MouseWheelScrolled)
	{
		this->m_ptr_GUIRepo->ptr_SFEvent->type = sf::Event::GainedFocus;

		if (this->m_ptr_GUIRepo->ptr_SFEvent->mouseWheel.x < 0)
		{
			if (this->m_CursorSpace == 0.5f)
			{
				return;
			}
			if (this->m_CursorSpace <= MIN_CURSOR_SPACE)
			{
				this->m_CursorSpace -= SMALL_INCREMENT;
			}
			else if (this->m_CursorSpace <= MAX_CURSOR_SPACE)
			{
				this->m_CursorSpace -= LARGE_INCREMENT;
			}
			else
			{
				this->m_CursorSpace -= MAX_CURSOR_SPACE;
			}
		}
		else
		{
			if (this->m_CursorSpace < MIN_CURSOR_SPACE)
			{
				this->m_CursorSpace += SMALL_INCREMENT;
			}
			else if (this->m_CursorSpace < MAX_CURSOR_SPACE)
			{
				this->m_CursorSpace += LARGE_INCREMENT;
			}
			else
			{
				this->m_CursorSpace += MAX_CURSOR_SPACE;
			}
		}
	}
}

void spe::UIAnimationEditor::RenderKeyFrames()
{
	const static float s_sizeTo0Frame = 100;
	const static float s_keyFrameSize = 18;
	const std::vector<spe::KeyFrame>& frames = this->m_ptr_Anim->GetkeyFrames();
	const float y = ImGui::GetCursorPosY() + 40;
	float currentMs = 0;
	float add = 0;
	int cnt = 0;
	std::string keyFrameTimeLine = std::string(10, ' ');

	ImGui::NewLine();
	ImGui::SetCursorPos(ImVec2(s_sizeTo0Frame , y));

	for (int i = 0; i <= this->m_ptr_Anim->GetAnimationTime(); i++)
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
				this->m_KeyFrameSelected.isClicked = true;
				this->m_KeyFrameSelected.keyFrameSelected = &frames[cnt];
				this->m_KeyFrameSelected.position = i;
			}
			ImGui::PopStyleColor();

			currentMs = 0;
			cnt++;
		}
		currentMs++;

		ImGui::SetCursorPos(ImVec2(100.0f + ((int)i + 1.0f) * this->m_CursorSpace, y));
	}
	ImGui::NewLine();
	ImGui::Dummy(ImVec2(0, 20));
}

void spe::UIAnimationEditor::AddKeyFrame()
{
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 2 + ImGui::GetScrollX(), ImGui::GetCursorPosY()));
	if (ImGui::Button("Add KeyFrame"))
	{
		if (this->m_ptr_GUIRepo->sprite_in_inspector != nullptr)
		{
			this->m_FrameAdder.KeyFramePath = this->m_ptr_GUIRepo->sprite_in_inspector->SpriteRenderer.Path;
		}
		this->m_FrameAdder.IsKeyFrameMenuOpen = true;
		this->m_FrameAdder.SetAnimation(this->m_ptr_Anim);
	}
	ImGui::SameLine();
	spe::Style::DisplaySmybolAsText(ICON_FA_PLUS);
}

void spe::UIAnimationEditor::SaveAnimation()
{
	if (ImGui::Button("Save"))
	{
		spe::Savesystem::CreateAnimationSaveFile(this->m_ptr_GUIRepo->sprite_in_inspector, *this->m_ptr_Anim);
	}
	spe::UIUtility::SameLine(2);
	spe::Style::DisplaySmybolAsText(ICON_FA_SAVE);
}

void spe::UIAnimationEditor::RenameAnimation()
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
			// Delete the old file
			spe::Utility::Delete(this->m_ptr_Anim->GetPath());

			this->m_ptr_GUIRepo->sprite_in_inspector->Animator.SetName(std_name, this->m_ptr_Anim->GetName());
			// Getting the new animation becuase the old 1 got deleted
			this->m_ptr_Anim = &this->m_ptr_GUIRepo->sprite_in_inspector->Animator.Animations[std_name];
			
			spe::Savesystem::CreateAnimationSaveFile(this->m_ptr_GUIRepo->sprite_in_inspector, *this->m_ptr_Anim);

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
	this->ZoomEditorTimeLine();
	this->BeginWindow();
	this->RenderTimeLineRealTimePoint();
	this->EditorTimeLine();
	this->AddKeyFrame();

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
	ImGui::Text("Loop");
	ImGui::SameLine();
	ImGui::Checkbox("##loop", &this->m_ptr_Anim->Loop);

	this->SaveAnimation();
	this->RenameAnimation();

	this->CloseWindow();


	if (this->m_FrameAdder.IsKeyFrameMenuOpen)
	{
		this->m_FrameAdder.Render();
	}
}

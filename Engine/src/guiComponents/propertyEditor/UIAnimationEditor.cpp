#include "UIAnimationEditor.h"

//Constructor

s2d::UIAnimationEditor::UIAnimationEditor()
{
	// 100 by default, will increase with the animation size
	this->m_keyFramesToEdit = 10000;
	this->m_anim = nullptr;
	this->display = false;
	this->m_keyFrameSelected.keyFrameSelected = nullptr;
	this->is_hovered = false;
	this->m_cursor_space = 30;
	this->m_ptr_event_engine = nullptr;
	this->m_ptr_repo = nullptr;
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
	std::string name = "Editor - " + this->m_anim->getName();

	ImGui::SetCursorPos(ImVec2(8 + ImGui::GetScrollX(), 15));
	ImGui::Text(name.c_str());

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 850 + ImGui::GetScrollX(), ImGui::GetCursorPosY() - 38));

	//Close button
	if (ImGui::Button("x"))
	{
		this->m_anim->ptr_applied_sprite->animator.stop(this->m_anim->getName());

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
			this->m_anim->ptr_applied_sprite->animator.play(this->m_anim->getName());
		}
	}
	else
	{
		if (s2d::FontManager::displaySmybolAsButton(ICON_FA_SQUARE))
		{
			this->m_anim->ptr_applied_sprite->animator.stop(this->m_anim->getName());
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

bool s2d::UIAnimationEditor::displayTimeFrameBasedOnCursorSpace(size_t i_pos)
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

void s2d::UIAnimationEditor::displayKeyFrameInfo()
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
		this->m_anim->stop();
		this->m_anim->deleteKeyFrame(this->m_keyFrameSelected.position);
		this->m_keyFrameSelected.keyFrameSelected = nullptr;
	}
	ImGui::SameLine();
	s2d::FontManager::displaySmybolAsText(ICON_FA_TRASH);

	ImGui::SetCursorPos(pos);
}

bool s2d::UIAnimationEditor::renderTextBasedOnScroll(size_t i)
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

void s2d::UIAnimationEditor::renderTimeLineRealTimePoint()
{
	const ImVec2 cursor_pos = ImGui::GetCursorPos();
	ImGui::SetCursorPosX(100);
	const float y = ImGui::GetCursorPosY() + 5;
	bool b = false;
	for (float i = 0; i < this->m_anim->total_time_passed * 100; i += 0.5f)
	{	
		ImGui::SetCursorPos(ImVec2(100 + ((int)i + 1.0f) * this->m_cursor_space, y));
	}
	ImGui::Text("|");

	ImGui::SetCursorPos(cursor_pos);
}

void s2d::UIAnimationEditor::zoomEditorTimeLine()
{
	if (this->m_ptr_event_engine->type == sf::Event::MouseWheelScrolled)
	{
		this->m_ptr_event_engine->type = sf::Event::GainedFocus;

		if (this->m_ptr_event_engine->mouseWheel.x < 0)
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

	for (int i = 0; i <= this->m_anim->getAnimationTime(); i++)
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

void s2d::UIAnimationEditor::addKeyFrame()
{
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 2 + ImGui::GetScrollX(), ImGui::GetCursorPosY()));
	if (ImGui::Button("Add KeyFrame"))
	{
		if (this->m_ptr_repo->sprite_in_inspector != nullptr)
		{
			this->keyFrameAdder.key_frame_path = this->m_ptr_repo->sprite_in_inspector->sprite_renderer.path;
		}
		this->keyFrameAdder.is_key_frame_menu_open = true;
		this->keyFrameAdder.setAnimation(this->m_anim);
	}
	ImGui::SameLine();
	s2d::FontManager::displaySmybolAsText(ICON_FA_PLUS);

	this->is_hovered = this->keyFrameAdder.is_hovered;
	if (!this->is_hovered)
	{
		this->is_hovered = s2d::UI::isHovered(ImGui::GetWindowSize(), ImGui::GetWindowPos());
	}
}
void s2d::UIAnimationEditor::saveAnimation()
{
	if (ImGui::Button("Save"))
	{
		s2d::flc::createAnimationSaveFile(this->m_ptr_repo->sprite_in_inspector, *this->m_anim);
	}
	s2d::UI::sameLine(2);
	s2d::FontManager::displaySmybolAsText(ICON_FA_SAVE);
}

void s2d::UIAnimationEditor::renameAnimation()
{
	static std::string s_renamed_pop_up_name = "";

	if (ImGui::Button("Rename"))
	{
		ImGui::OpenPopup("First");
	}

	if (ImGui::BeginPopup("First"))
	{
		ImGui::Text("Rename");
		ImGui::Separator();
		ImGui::InputTextWithHint("##rename-anim", "<name>", &s_renamed_pop_up_name[0], CHAR_MAX);

		s2d::UI::sameLine(2);
		
		if (s2d::FontManager::displaySmybolAsButton(ICON_FA_ARROW_RIGHT))
		{
			this->m_ptr_repo->sprite_in_inspector->animator.setName(s_renamed_pop_up_name.c_str(), this->m_anim->getName());
			s_renamed_pop_up_name = "";
		}
		
		this->is_hovered = s2d::UI::isHovered(ImGui::GetWindowSize(), ImGui::GetWindowPos());

		ImGui::EndPopup();
	}

	s2d::UI::sameLine(2);
	s2d::FontManager::displaySmybolAsText(ICON_FA_EDIT);
}

// Public methods

void s2d::UIAnimationEditor::displayEditor()
{
	if (this->m_ptr_repo->sprite_in_inspector == nullptr)
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
	ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size - 0.2f);
	ImGui::Checkbox("##loop", &this->m_anim->loop);
	ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);

	this->saveAnimation();
	this->renameAnimation();

	this->closeWindow();


	if (this->keyFrameAdder.is_key_frame_menu_open)
	{
		this->keyFrameAdder.update();
	}
}

void s2d::UIAnimationEditor::setSpriteRepository(s2d::SpriteRepository& repo)
{
	this->m_ptr_repo = &repo;
	this->keyFrameAdder.setSpriteRepository(repo);
}

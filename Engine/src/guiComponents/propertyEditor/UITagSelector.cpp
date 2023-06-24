#include "UITagSelector.h"

// Constructor / Destructor

s2d::UITagSelector::UITagSelector()
{
	this->m_ptr_sprite_repo = nullptr;
	this->m_tag_name[0] = '\0';
	this->m_open_name_popup = false;	
}

// Public functions

void s2d::UITagSelector::update(bool* hovered)
{
	this->renderSelector();
	this->renderPopup(hovered);
}

// Private functions

void s2d::UITagSelector::renderSelector()
{
	const std::vector<std::string>& items = this->m_ptr_sprite_repo->getTags();
	static int selectedItem = 0;

	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMax().x - 120, 40));

	ImGui::SetNextItemWidth(110);
	const char* tag = this->m_ptr_sprite_repo->sprite_in_inspector->tag.c_str();
	if (ImGui::BeginCombo("##dropdown-tags", tag))
	{
		for (int i = 0; i < items.size(); i++) 
		{
			bool isSelected = (selectedItem == i);

			ImGui::SetNextItemWidth(110);
			if (ImGui::Selectable(items[i].c_str(), isSelected))
			{
				selectedItem = i;
				this->m_ptr_sprite_repo->sprite_in_inspector->tag = items[i];
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::Separator();
		if (ImGui::Button("Add tag"))
		{
			this->m_open_name_popup = true;
			ImGui::SetNextWindowFocus();
		}
		ImGui::EndCombo();
	}
}

void s2d::UITagSelector::renderPopup(bool* hovered)
{
	if (!this->m_open_name_popup)
	{
		return;
	}
	const ImVec2 size = ImVec2(200, 50);

	ImGui::Begin("##tag-adder", NULL, COMMON_FLAGS);

	ImGui::SetNextItemWidth(size.x);
	ImGui::InputTextWithHint("##add-text-input", "<name>", this->m_tag_name, CHAR_MAX);

	s2d::UI::setWindowScreenMiddle(size);
	ImGui::SetWindowSize(size);
	*hovered = s2d::UI::isHovered(ImGui::GetWindowPos(), size);
	ImGui::End();

	if (ImGui::IsKeyReleased(ImGuiKey_Enter))
	{
		this->m_ptr_sprite_repo->addTag(std::string(this->m_tag_name));

		this->m_open_name_popup = '\0';
		this->m_open_name_popup = false;
	}

	if (ImGui::IsKeyReleased(ImGuiKey_Escape))
	{
		this->m_open_name_popup = '\0';
		this->m_open_name_popup = false;
	}
}



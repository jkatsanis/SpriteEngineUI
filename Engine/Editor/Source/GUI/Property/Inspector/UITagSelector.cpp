#include "UITagSelector.h"

// Constructor / Destructor

spe::UITagSelector::UITagSelector()
{
	this->Init();
}

void spe::UITagSelector::Init()
{
	this->m_TagName[0] = '\0';
	this->m_OpenNamePopup = false;
}

// Public functions

void spe::UITagSelector::Render()
{
	this->RenderSelector();
	this->RenderPopup();
}

// Private functions

void spe::UITagSelector::RenderSelector()
{
	const std::vector<std::string>& items = this->m_ptr_Repo->Tags;
	static int selectedItem = 0;

	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMax().x - 120, 40));

	ImGui::SetNextItemWidth(110);
	const char* tag = this->m_ptr_GUIRepo->sprite_in_inspector->Tag.c_str();
	if (ImGui::BeginCombo("##dropdown-tags", tag))
	{
		for (int i = 0; i < items.size(); i++) 
		{
			bool isSelected = (selectedItem == i);

			ImGui::SetNextItemWidth(110);
			if (ImGui::Selectable(items[i].c_str(), isSelected))
			{
				selectedItem = i;
				this->m_ptr_GUIRepo->sprite_in_inspector->Tag = items[i];
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::Separator();
		if (ImGui::Button("Add tag"))
		{
			this->m_OpenNamePopup = true;
			ImGui::SetNextWindowFocus();
		}
		ImGui::EndCombo();
	}
}

void spe::UITagSelector::RenderPopup()
{
	if (!this->m_OpenNamePopup)
	{
		return;
	}
	const ImVec2 size = ImVec2(200, 50);

	ImGui::Begin("##tag-adder", NULL, DEFAULT_FLAGS);

	ImGui::SetNextItemWidth(size.x);
	ImGui::InputTextWithHint("##add-text-input", "<name>", this->m_TagName, CHAR_MAX);

	spe::UIUtility::SetWindowScreenMiddle(size);
	ImGui::SetWindowSize(size);
	this->Hovered = spe::UIUtility::IsHovered(ImGui::GetWindowPos(), size);
	ImGui::End();

	if (ImGui::IsKeyReleased(ImGuiKey_Enter))
	{
		this->m_ptr_Repo->Tags.push_back(std::string(this->m_TagName));
		this->Hovered = false;
		this->m_OpenNamePopup = '\0';
		this->m_OpenNamePopup = false;
	}

	if (ImGui::IsKeyReleased(ImGuiKey_Escape))
	{
		this->m_OpenNamePopup = '\0';
		this->m_OpenNamePopup = false;
	}
}



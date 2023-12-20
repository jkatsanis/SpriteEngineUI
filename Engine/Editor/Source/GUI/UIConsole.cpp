#include "UIConsole.h"

// Ctor

spe::UIConsole::UIConsole()
{
	this->Buffer = "";
}

// Private

void spe::UIConsole::Init()
{
	spe::Log::SetStringBuffer(&this->Buffer);
}

// Public

void spe::UIConsole::Render()
{
	const float x = this->m_ptr_GUIRepo->InspectorData.ptr_Size->x;

	ImGui::Begin("##Console", NULL, DEFAULT_FLAGS);

	ImGui::Text("Console");
	ImGui::Separator();

	const ImVec2 temp = ImGui::GetCursorPos();
	ImGui::SetCursorPosX(x - 125);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 30);


	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 1));

	if(spe::Style::DisplaySymbolInMenuItemWithText(ICON_FA_TRASH, "Clear", 5))
	{
		this->Buffer = "";
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("Clear the console");
	}

	ImGui::PopStyleColor(2);
	ImGui::SetCursorPos(temp);

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
	ImGui::TextUnformatted(this->Buffer.c_str());

	ImGui::SetWindowSize(ImVec2(x, 480));

	ImGui::SetWindowPos(ImVec2(1920 - x, 600));

	this->Hovered = spe::UIUtility::IsHovered(ImGui::GetWindowPos(), ImGui::GetWindowSize());

	ImGui::End();
}

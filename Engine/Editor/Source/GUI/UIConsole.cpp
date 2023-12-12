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
	ImGui::Begin("##Console", NULL, DEFAULT_FLAGS);
	ImGui::Text("Console");
	ImGui::Separator();

	ImGui::TextUnformatted(this->Buffer.c_str());

	float x =  this->m_ptr_GUIRepo->InspectorData.ptr_Size->x;
	ImGui::SetWindowSize(ImVec2(x, 480));

	ImGui::SetWindowPos(ImVec2(1920 - x, 600));

	this->Hovered = spe::UIUtility::IsHovered(ImGui::GetWindowPos(), ImGui::GetWindowSize());

	ImGui::End();
}

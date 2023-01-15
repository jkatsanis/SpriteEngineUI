#include "GUIManager.h"

void s2d::GUIManager::update()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4 shadow_color = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);

	//Setting it centered
	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style->FramePadding = ImVec2(8, 6);

	style->Colors[ImGuiCol_TitleBg] = ImColor(49, 49, 76);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(49, 49, 76);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(217, 101, 53, 255);

	//the sprite selected
	style->Colors[ImGuiCol_Header] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_HeaderHovered] = ImColor(100, 90, 100, 100);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(0, 0, 0, 0);

	style->Colors[ImGuiCol_Button] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(15, 15, 25);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(100, 90, 100, 100);

	//The background of (input) 
	style->Colors[ImGuiCol_FrameBg] = shadow_color;
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(45, 45, 45);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(50, 50, 50);

	style->Colors[ImGuiCol_WindowBg] = ImColor(22, 22, 32, 255);

	style->Colors[ImGuiCol_TextSelectedBg] = ImColor(30, 30, 30);

	style->Colors[ImGuiCol_SliderGrab] = ImColor(30, 30, 30);
	style->Colors[ImGuiCol_SliderGrabActive] = ImColor(25, 25, 25);

	style->Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255);

	style->FrameRounding = 4.0f;
	style->GrabRounding = 4.0f;
}

float s2d::GUIManager::s_fontSize = 2.0f;

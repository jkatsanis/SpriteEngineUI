#include "UIWindow.h"

//Constructor

s2d::UIWindow::UIWindow()
{
	this->areAnyUIWindowsHovered = false;
}

// Static functions

void s2d::UIWindow::renderStyle(ImGuiStyle* style)
{
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

	style->Colors[ImGuiCol_WindowBg] = ImColor(36, 36, 36);

	style->Colors[ImGuiCol_TextSelectedBg] = ImColor(30, 30, 30);

	style->Colors[ImGuiCol_SliderGrab] = ImColor(30, 30, 30);
	style->Colors[ImGuiCol_SliderGrabActive] = ImColor(25, 25, 25);

	style->Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255);

	style->Colors[ImGuiCol_ChildBg] = ImColor(26, 26, 26);

	style->FrameRounding = 4.0f;
	style->GrabRounding = 4.0f;

}

//Public functions

void s2d::UIWindow::update()
{
	//When we press play we need to save our data again, lol
	this->m_UIToolButtons.setBackgroundColorToSave(this->m_UIInspector.backgroundColor);

	s2d::UIWindow::renderStyle(&ImGui::GetStyle());
	this->m_UIHierarchy.displayHierarchyWindow();
	this->m_UIToolButtons.createToolsAndButtons();
	this->m_UIInspector.createUIInspector();
	this->m_UIAnimation.createUIAnimationWindow();
	this->m_UIAssetFolder.createAssetLinkerWindow();

	if (this->m_UIHierarchy.isHovered || this->m_UIToolButtons.isHovered || this->m_UIInspector.isHovered || this->m_UIAssetFolder.isHovered || this->m_UIAnimation.isHovered)
	{
		this->areAnyUIWindowsHovered = true;
	}
	else
	{
		this->areAnyUIWindowsHovered = false;
	}
}

void s2d::UIWindow::init(s2d::SpriteRepository& repo)
{
	this->m_ptr_repo = &repo;
	this->areAnyUIWindowsHovered = false;
	this->m_UIHierarchy = s2d::UIHierarchy(repo);
	this->m_UIInspector.setSpriteRepository(repo);
	this->m_UIAssetFolder.setSpriteRepository(repo);
	this->m_UIAnimation.setSpriteRepository(repo);
	this->m_UIToolButtons = s2d::UIToolButtons(repo);
}


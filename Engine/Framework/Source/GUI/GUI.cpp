#include "GUI.h"

bool spe::GUI::Button(const std::string& content)
{
    return ImGui::Button(content.c_str());
}

void spe::GUI::SetCursorCamera(const spe::Vector2& position)
{
    ImVec2 new_cursor = ImVec2(spe::GUI::s_ptr_Camera->Position.X * -1 + 960, 540 + spe::GUI::s_ptr_Camera->Position.Y * -1);
    new_cursor.x += position.X;
    new_cursor.y -= position.Y;
    
    ImGui::SetCursorPos(new_cursor);
}

void spe::GUI::SetCursor(const spe::Vector2& position)
{
    ImVec2 new_cursor = ImVec2(960, 540);
    new_cursor.x += position.X;
    new_cursor.y -= position.Y;

    ImGui::SetCursorPos(new_cursor);
}

void spe::GUI::SetFontScale(float scale)
{
    ImGui::SetWindowFontScale(scale);
}

void spe::GUI::Window(const std::string& id, const spe::Vector2& pos, const spe::Vector2& size)
{
    ImGui::Begin(id.c_str(), NULL, DEFAULT_FLAGS);
    ImGui::SetWindowSize(spe::Vector2::toImVec2(size));
    ImGui::SetWindowPos(ImVec2(960 + pos.X, 540 - pos.Y));
}

void spe::GUI::End()
{
    ImGui::End();
}

const spe::Camera* spe::GUI::s_ptr_Camera = nullptr;
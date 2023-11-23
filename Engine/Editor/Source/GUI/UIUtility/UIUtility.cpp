#include "UIUtility.h"

void spe::UIUtility::UpdateCursor()
{
    spe::UIUtility::GUICursor = spe::Vector2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
}

void spe::UIUtility::DrawRectangleInGUIWIndow(const ImVec2& size, const ImVec2& top_left, const ImColor& color)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(top_left, ImVec2(top_left.x + size.x, top_left.y - size.y), color);
}

bool spe::UIUtility::IsHovered(const ImVec2& windowPos, const ImVec2& windowSize)
{
    ImVec2 mousePos = ImVec2(spe::UIUtility::GUICursor.x, spe::UIUtility::GUICursor.y);

    return mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
        mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y;
}

bool spe::UIUtility::HandleCloseAndReloadWindow(bool open, bool& reload, bool& hovered, ImVec2& window_size, const ImVec2& original_size)
{
    bool must_return = false;
    if (!open)
    {
        must_return = true;
        hovered = false;
    }
    if (reload)
    {
        // Default window size
        window_size = original_size;
        reload = false;
    }
    return must_return;
}

// Static init

spe::Vector2 spe::UIUtility::GUICursor = spe::Vector2(0, 0);
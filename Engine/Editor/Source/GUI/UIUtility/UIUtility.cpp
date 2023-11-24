#include "UIUtility.h"

void spe::UIUtility::UpdateCursor()
{
    spe::UIUtility::GUICursor = spe::Vector2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
}

bool spe::UIUtility::RenderCloseRectangle(float padding_left, const char* icon, const std::string& id, const std::string& content, float cursor_pos)
{
    bool close = true;
    const static float CLOSE_RECTANGLE_INNER_PADDING = 7;
    const static ImVec2 SIZE = ImVec2(150, 32);

    ImGui::SetCursorPosX(padding_left);
    ImGui::SetCursorPosY(cursor_pos);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.085f, 0.085f, 0.085f, 1.0f));
    ImGui::BeginChild(id.c_str(), SIZE);
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + CLOSE_RECTANGLE_INNER_PADDING, ImGui::GetCursorPosY() + CLOSE_RECTANGLE_INNER_PADDING));

    spe::Style::DisplaySmybolAsText(icon);
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 30
        , ImGui::GetCursorPosY() - ImGui::CalcTextSize(icon).y - 1));
    ImGui::Text(content.c_str());
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 30, 0));
    if (ImGui::Button("x"))
    {
        // Returns false so we can do ourWindow = renderCloseRectangle() and when ourWindow is false it will stop rendering
        close = false;
    }
    ImGui::PopStyleColor();
    ImGui::EndChild();
    return close;
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

bool spe::UIUtility::HandleCloseAndReloadWindow(spe::UIWindowData& data, bool& hovered, const ImVec2& original_size)
{
    bool must_return = false;
    if (!data.IsOpen)
    {
        must_return = true;
        hovered = false;
    }
    if (data.Reload)
    {
        // Default window size
        *data.ptr_Size = original_size;
        data.Reload = false;
    }
    return must_return;
}

// Static init

spe::Vector2 spe::UIUtility::GUICursor = spe::Vector2(0, 0);
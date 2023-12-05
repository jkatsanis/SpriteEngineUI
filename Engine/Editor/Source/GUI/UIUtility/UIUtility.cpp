#include "UIUtility.h"

// Private

float spe::UIUtility::scaleChanger(spe::ScaleDott& dott, float default_size, float pos_o, bool x)
{
    if (spe::UIUtility::isCursorClickedOnRectangle(dott.ptr_scaling_rectangle->Shape))
    {
        dott.clicked = true;
    }
    if (dott.clicked && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2f pos = (x)
            ? sf::Vector2f(spe::UIUtility::getWorldCordinates().X, dott.ptr_scaling_rectangle->Shape.getPosition().y)
            : sf::Vector2f(dott.ptr_scaling_rectangle->Shape.getPosition().x,spe::UIUtility::getWorldCordinates().Y);

        dott.ptr_scaling_rectangle->Shape.setPosition(pos);
        float scale = INVALID_SCALE;
        if (x)
        {
            pos.x -= 960;
            scale = (pos.x - pos_o) / (default_size / 2);
        }
        else
        {
            pos.y -= 540;
            scale = (pos.y + pos_o) / (default_size / 2);
        }
        return scale;
    }
    return INVALID_SCALE;
}

// Public

void spe::UIUtility::UpdateCursor()
{
    spe::UIUtility::WorldCursor.Position = spe::UIUtility::getWorldCordinates();
    spe::UIUtility::WorldCursor.SetLastPosition();

    spe::UIUtility::GUICursor.Position = spe::Vector2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
    spe::UIUtility::GUICursor.SetLastPosition();
}

float spe::UIUtility::xScaleChanger(spe::ScaleDott& dott, float default_size, float pos_x)
{
    return spe::UIUtility::scaleChanger(dott, default_size, pos_x, true);
}

float spe::UIUtility::yScaleChanger(spe::ScaleDott& dott, float default_size, float pos_y)
{
    return spe::UIUtility::scaleChanger(dott, default_size, pos_y, false);
}

bool spe::UIUtility::isCursorClickedOnSprite(const spe::Sprite* check)
{
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        return false;
    }
    sf::Vector2i cursorPos = sf::Mouse::getPosition(*spe::UIUtility::s_m_ptr_Window);
    spe::UIUtility::WorldCursor.Position = spe::UIUtility::s_m_ptr_Window->mapPixelToCoords(cursorPos);

    float getPosX = check->Transform.GetOrigininalPosition().X;
    float getPosY = check->Transform.GetOrigininalPosition().Y;

    float otherGetPosX = spe::UIUtility::WorldCursor.Position.X;
    float otherGetPosY = spe::UIUtility::WorldCursor.Position.Y;

    return (getPosX + check->Transform.TextureSize.X >= otherGetPosX
        && getPosX <= otherGetPosX + CURSOR_HITBOX
        && getPosY + check->Transform.TextureSize.Y >= otherGetPosY
        && getPosY <= otherGetPosY + CURSOR_HITBOX);
}

spe::Vector2 spe::UIUtility::getWorldCordinates()
{
    const sf::Vector2i cursorPos = sf::Mouse::getPosition(*spe::UIUtility::s_m_ptr_Window);
    const sf::Vector2f cursorWorldPos = spe::UIUtility::s_m_ptr_Window->mapPixelToCoords(cursorPos);

    return spe::Vector2(cursorWorldPos.x, cursorWorldPos.y);
}

bool spe::UIUtility::isCursorClickedOnRectangle(const sf::RectangleShape& shape)
{
    if (spe::Event::MousePressedLeft != spe::UIUtility::s_m_ptr_Event->Type)
    {
        return false;
    }

    float getPosX = shape.getPosition().x;
    float getPosY = shape.getPosition().y;

    float getTextureSizeX = shape.getSize().x;
    float getTextureSizeY = shape.getSize().y;

    float otherGetPosX = spe::UIUtility::WorldCursor.Position.X;
    float otherGetPosY = spe::UIUtility::WorldCursor.Position.Y;

    bool collided = (getPosX + getTextureSizeX >= otherGetPosX
        && getPosX <= otherGetPosX + CURSOR_HITBOX
        && getPosY + getTextureSizeY >= otherGetPosY
        && getPosY <= otherGetPosY + CURSOR_HITBOX);

    if (collided)
        spe::UIUtility::s_m_ptr_Event->Type = spe::Event::None;

    return collided;
}

void spe::UIUtility::SetWindowScreenMiddle(const ImVec2& ref)
{
    ImVec2 size = spe::Vector2::toImVec2(ref);
    ImVec2 newPos = ImVec2(spe::Vector2::
        toImVec2(spe::Vector2(spe::Vector2::SCREEN_MIDDLE.X - size.x / 2,
            spe::Vector2::SCREEN_MIDDLE.Y - size.y / 2)));

    ImGui::SetWindowSize(size);
    ImGui::SetWindowPos(newPos);
}

void spe::UIUtility::SameLine(float width)
{
    ImGui::SameLine();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + width);
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
    ImVec2 mousePos = ImVec2(spe::UIUtility::GUICursor.Position.X, spe::UIUtility::GUICursor.Position.Y);

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
    
spe::UITransform spe::UIUtility::GUICursor;;
spe::UITransform spe::UIUtility::WorldCursor;

spe::Event* spe::UIUtility::s_m_ptr_Event = nullptr;
sf::RenderWindow* spe::UIUtility::s_m_ptr_Window = nullptr;
bool spe::UIUtility::s_IsAnyHovered = false;

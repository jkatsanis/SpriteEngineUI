#include "UI.h"

// Static init

short s2d::UI::s_cursor_hit_box = 20;
const sf::RenderWindow* s2d::UI::s_ptr_Window = nullptr;
sf::Vector2f s2d::UI::s_cursor_world_pos = sf::Vector2f(0, 0);
s2d::Transform s2d::UI::s_game_cursor = s2d::Transform();
s2d::Transform s2d::UI::s_gui_cursor = s2d::Transform();
s2d::Event* s2d::UI::s_event = nullptr;

// Private functions

float s2d::UI::scaleChanger(s2d::ScaleDott& dott, float default_size, float pos_o, bool x)
{
    if (s2d::UI::isCursorClickedOnRectangle(dott.ptr_scaling_rectangle->shape))
    {
        dott.clicked = true;
    }
    if (dott.clicked && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2f pos = (x)
            ? sf::Vector2f(s2d::UI::getWorldCordinates().x, dott.ptr_scaling_rectangle->shape.getPosition().y)
            : sf::Vector2f(dott.ptr_scaling_rectangle->shape.getPosition().x, s2d::UI::getWorldCordinates().y);

        dott.ptr_scaling_rectangle->shape.setPosition(pos);
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

// Public functions

void s2d::UI::update()
{
    // Transform postion changer cursor
    s2d::UI::s_game_cursor.position = s2d::UI::getWorldCordinates();
    s2d::UI::setCursorToWorldCoordinates(s2d::UI::s_game_cursor.position);

    // Imgui & resize cursor
    s2d::UI::s_gui_cursor.position = s2d::Vector2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
    s2d::UI::s_gui_cursor.setLastPosition();
}

bool s2d::UI::isCursorClickedOnSprite(const s2d::Sprite* check)
{
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        return false;
    }
    sf::Vector2i cursorPos = sf::Mouse::getPosition(*s2d::UI::s_ptr_Window);
    s2d::UI::s_cursor_world_pos = s2d::UI::s_ptr_Window->mapPixelToCoords(cursorPos);

    float getPosX = check->getOrigininalPosition().x;
    float getPosY = check->getOrigininalPosition().y;

    float otherGetPosX = s2d::UI::s_cursor_world_pos.x;
    float otherGetPosY = s2d::UI::s_cursor_world_pos.y;

    return (getPosX + check->transform.texture_size.x >= otherGetPosX
        && getPosX <= otherGetPosX + s2d::UI::s_cursor_hit_box
        && getPosY + check->transform.texture_size.y >= otherGetPosY
        && getPosY <= otherGetPosY + s2d::UI::s_cursor_hit_box);

}

s2d::Vector2 s2d::UI::getWorldCordinates()
{
    const sf::Vector2i cursorPos = sf::Mouse::getPosition(*s2d::UI::s_ptr_Window);
    const sf::Vector2f cursorWorldPos = s2d::UI::s_ptr_Window->mapPixelToCoords(cursorPos);

    return s2d::Vector2(cursorWorldPos.x, cursorWorldPos.y);
}

void s2d::UI::setCursorToWorldCoordinates(const s2d::Vector2& vec)
{   
    s2d::UI::s_game_cursor.position = s2d::Vector2(vec.x, vec.y);
    s2d::UI::s_game_cursor.setLastPosition();
}

bool s2d::UI::isCursorClickedOnRectangle(const sf::RectangleShape& shape)
{
    if (s2d::Event::MousePressedLeft != s2d::UI::s_event->type)
    {
        return false;
    }
    sf::Vector2i cursorPos = sf::Mouse::getPosition(*s2d::UI::s_ptr_Window);
    s2d::UI::s_cursor_world_pos = s2d::UI::s_ptr_Window->mapPixelToCoords(cursorPos);

    float getPosX = shape.getPosition().x;
    float getPosY = shape.getPosition().y;

    float getTextureSizeX = shape.getSize().x;
    float getTextureSizeY = shape.getSize().y;

    float otherGetPosX = s2d::UI::s_cursor_world_pos.x;
    float otherGetPosY = s2d::UI::s_cursor_world_pos.y;

    bool collided = (getPosX + getTextureSizeX >= otherGetPosX
        && getPosX <= otherGetPosX + s2d::UI::s_cursor_hit_box
        && getPosY + getTextureSizeY >= otherGetPosY
        && getPosY <= otherGetPosY + s2d::UI::s_cursor_hit_box);

    if(collided)
        s2d::UI::s_event->type = s2d::Event::None;

    return collided;
}

void s2d::UI::setRenderWindow(const sf::RenderWindow* window)
{
    s2d::UI::s_ptr_Window = window;
}

void s2d::UI::setS2DEvent(s2d::Event* const event)
{
    s2d::UI::s_event = event;
}

std::string s2d::UI::getUserProjectPathSeperatetFromEnginePath(const std::string& path)
{
    std::string userProject = s2d::EngineData::s_name_of_user_project;
    int idx = (int)path.find(userProject);
    std::string finalStr = "";
   // std::cout << path << std::endl;
    for (int i = idx + (int)userProject.size() + 1; i < path.size(); i++)
    {
        finalStr += path[i];
    }
    return finalStr;
}

void s2d::UI::setWindowScreenMiddle(const s2d::Vector2& windowSize)
{
    ImVec2 size = s2d::Vector2::toImVec2(windowSize);
    ImVec2 newPos = ImVec2(s2d::Vector2::
        toImVec2(s2d::Vector2(s2d::Vector2::SCREEN_MIDDLE.x - size.x / 2, 
            s2d::Vector2::SCREEN_MIDDLE.y - size.y / 2)));

    ImGui::SetWindowSize(size);
    ImGui::SetWindowPos(newPos);
}

bool s2d::UI::isHovered(const ImVec2& windowPos, const ImVec2& windowSize)
{
    ImVec2 mousePos = ImVec2(s2d::UI::s_gui_cursor.position.x, s2d::UI::s_gui_cursor.position.y);

    return mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
        mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y;
}

bool s2d::UI::renderCloseRectangle(float paddingLeft, const char* icon, const std::string& id, const std::string& content, float cursorPos)
{
    bool close = true;
    const static float CLOSE_RECTANGLE_INNER_PADDING = 7;
    const static ImVec2 SIZE = ImVec2(150, 32);

    ImGui::SetCursorPosX(paddingLeft);
    ImGui::SetCursorPosY(cursorPos);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.085f, 0.085f, 0.085f, 1.0f));
    ImGui::BeginChild(id.c_str(), SIZE);
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + CLOSE_RECTANGLE_INNER_PADDING, ImGui::GetCursorPosY() + CLOSE_RECTANGLE_INNER_PADDING));

    s2d::FontManager::displaySmybolAsText(icon);
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 30
        , ImGui::GetCursorPosY() - ImGui::CalcTextSize(icon).y - 1));
    ImGui::Text(content.c_str());
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 30 , 0));
    if (ImGui::Button("x"))
    {
        // Returns false so we can do ourWindow = renderCloseRectangle() and when ourWindow is false it will stop rendering
        close = false;
    }
    ImGui::PopStyleColor();
    ImGui::EndChild();
    return close;
}

bool s2d::UI::handleCloseAndReloadWindow(bool open, bool& reload, bool& hovered, ImVec2& window_size, const ImVec2& original_size)
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

void s2d::UI::drawRectangleInGUIWIndow(const ImVec2& size, const ImVec2& top_left, const ImColor& color)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(top_left, ImVec2(top_left.x + size.x, top_left.y - size.y), color);
}

float s2d::UI::xScaleChanger(s2d::ScaleDott& dott, float default_size, float pos_x)
{
    return s2d::UI::scaleChanger(dott, default_size, pos_x, true);
}

float s2d::UI::yScaleChanger(s2d::ScaleDott& dott, float default_size, float pos_y)
{
    return s2d::UI::scaleChanger(dott, default_size, pos_y, false);
}

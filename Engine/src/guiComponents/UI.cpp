#include "UI.h"

bool s2d::UI::isCursorClickedOnSprite(const s2d::Sprite* check)
{
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        return false;
    }
    sf::Vector2i cursorPos = sf::Mouse::getPosition(*s2d::UI::s_ptr_Window);
    s2d::UI::s_cursorWorldPos = s2d::UI::s_ptr_Window->mapPixelToCoords(cursorPos);

    float getPosX = check->getOrigininalPosition().x;
    float getPosY = check->getOrigininalPosition().y;

    float otherGetPosX = s2d::UI::s_cursorWorldPos.x;
    float otherGetPosY = s2d::UI::s_cursorWorldPos.y;

    return (getPosX + check->transform.textureSize.x >= otherGetPosX
        && getPosX <= otherGetPosX + s2d::UI::s_cursorHitbox
        && getPosY + check->transform.textureSize.y >= otherGetPosY
        && getPosY <= otherGetPosY + s2d::UI::s_cursorHitbox);

}

s2d::Vector2 s2d::UI::getWorldCordinates()
{
    sf::Vector2i cursorPos = sf::Mouse::getPosition(*s2d::UI::s_ptr_Window);
    sf::Vector2f cursorWorldPos = s2d::UI::s_ptr_Window->mapPixelToCoords(cursorPos);

    return s2d::Vector2(cursorWorldPos.x, cursorWorldPos.y);
}

void s2d::UI::setCursorToWorldCoordinates(const s2d::Vector2& vec)
{   
    s2d::UI::cursor.position = s2d::Vector2(vec.x, vec.y);
    s2d::UI::cursor.setLastPosition();
}

bool s2d::UI::isCursorClickedOnRectangle(const sf::RectangleShape& shape)
{
    if (s2d::Event::MousePressedLeft != s2d::UI::s_event->type)
    {
        return false;
    }
    sf::Vector2i cursorPos = sf::Mouse::getPosition(*s2d::UI::s_ptr_Window);
    s2d::UI::s_cursorWorldPos = s2d::UI::s_ptr_Window->mapPixelToCoords(cursorPos);

    float getPosX = shape.getPosition().x;
    float getPosY = shape.getPosition().y;

    float getTextureSizeX = shape.getSize().x;
    float getTextureSizeY = shape.getSize().y;

    float otherGetPosX = s2d::UI::s_cursorWorldPos.x;
    float otherGetPosY = s2d::UI::s_cursorWorldPos.y;

    bool collided = (getPosX + getTextureSizeX >= otherGetPosX
        && getPosX <= otherGetPosX + s2d::UI::s_cursorHitbox
        && getPosY + getTextureSizeY >= otherGetPosY
        && getPosY <= otherGetPosY + s2d::UI::s_cursorHitbox);

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

void s2d::UI::setWindowScreenMiddle(const s2d::Vector2& windowSize)
{
    ImVec2 size = s2d::Vector2::toImVec2(windowSize);
    ImVec2 newPos = ImVec2(s2d::Vector2::
        toImVec2(s2d::Vector2(s2d::Vector2::SCREEN_MIDDLE.x - size.x / 2, 
            s2d::Vector2::SCREEN_MIDDLE.y - size.y / 2)));

    ImGui::SetWindowSize(size);
    ImGui::SetWindowPos(newPos);
}

short s2d::UI::s_cursorHitbox = 20;
const sf::RenderWindow* s2d::UI::s_ptr_Window = nullptr;
sf::Vector2f s2d::UI::s_cursorWorldPos = sf::Vector2f(0, 0);
s2d::Transform s2d::UI::cursor = s2d::Transform();
s2d::Event* s2d::UI::s_event = nullptr;
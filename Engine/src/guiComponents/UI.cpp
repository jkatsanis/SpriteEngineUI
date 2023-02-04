#include "UI.h"

bool s2d::UI::isCursorClickedOnSprite(const s2d::Sprite* check)
{
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

void s2d::UI::setRenderWindow(const sf::RenderWindow* window)
{
    s2d::UI::s_ptr_Window = window;
}

short s2d::UI::s_cursorHitbox = 20;
const sf::RenderWindow* s2d::UI::s_ptr_Window = nullptr;
sf::Vector2f s2d::UI::s_cursorWorldPos = sf::Vector2f(0, 0);
s2d::Transform s2d::UI::cursor = s2d::Transform();
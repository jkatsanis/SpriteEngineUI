#include "UIRealTimeEditorTransformPosition.h"

// Constructor


s2d::UIRealTimeEditorTransformPosition::UIRealTimeEditorTransformPosition()
{
    this->m_windowEvent = nullptr;
    this->m_isAnyUIWindowHovered = nullptr;
    this->m_ptr_Window = nullptr;
    this->m_clickedSprite = nullptr;
    this->m_ptr_Inspectorstate = nullptr;
    this->m_realeasedCursorOnSprite = false;
    this->m_clickedSpriteId = -1;
}

s2d::UIRealTimeEditorTransformPosition::UIRealTimeEditorTransformPosition(sf::RenderWindow* window, s2d::InspectorState* ptr_Inspectorstate, bool* isAnyUIWindowHovered, s2d::Event* windowEvent)
{
    this->m_windowEvent = windowEvent;
    this->m_isAnyUIWindowHovered = isAnyUIWindowHovered;
    this->m_ptr_Inspectorstate = ptr_Inspectorstate;
    this->m_ptr_Window = window;
    this->m_clickedSprite = nullptr;
    this->m_realeasedCursorOnSprite = false;
    this->m_clickedSpriteId = -1;
}

// Public functions

void s2d::UIRealTimeEditorTransformPosition::update()
{
    if (*this->m_isAnyUIWindowHovered)
    {
        return;
    }
    // Check if we click on a sprite in the editor
    s2d::Sprite* clickedSprite = this->checkIfMouseClickedOnSprite();

    this->m_cursorWorldPos = s2d::UI::getWorldCordinates();
    s2d::UI::setCursorToWorldCoordinates(this->m_cursorWorldPos);

    if (clickedSprite != nullptr && this->m_clickedSprite != nullptr)
    {
        this->moveComponent();
    }
    else
    {
        this->m_clickedSprite = nullptr;
    }

}

// Private functions

void s2d::UIRealTimeEditorTransformPosition::moveComponent()
{
    float x = this->m_cursorWorldPos.x - 960;
    float y = -(this->m_cursorWorldPos.y - 540);

    s2d::Vector2 pos = s2d::Vector2(this->m_cursorWorldPos.x - 960, -(this->m_cursorWorldPos.y - 540));

    float m = x - this->m_clickedSprite->transform.position.x;
    float my = y - this->m_clickedSprite->transform.position.y;

    if (s2d::UI::cursor.posiitonChanged)
    {
        s2d::Vector2 moved = s2d::UI::cursor.lastPos - s2d::UI::cursor.position;
        m += moved.x;
        my -= moved.y;
    }

    s2d::Vector2 newPosition = s2d::Vector2(x - m, y - my);

    this->m_clickedSprite->transform.position = newPosition;

}

// Check if clicked on a sprite

bool s2d::UIRealTimeEditorTransformPosition::checkClick(s2d::Sprite& sprite)
{
    bool collied = s2d::UI::isCursorClickedOnSprite(&sprite);


    if (collied && this->m_windowEvent->type == s2d::Event::Type::MousePressedLeft)
    {
        this->m_clickedSpriteId = sprite.getId();

        this->m_realeasedCursorOnSprite = true;
        return true;
    }
    if (this->m_windowEvent->type == s2d::Event::MouseReleasedLeft)
    {
        this->m_realeasedCursorOnSprite = false;
        return false;
    }


    return false;
}

s2d::Sprite* s2d::UIRealTimeEditorTransformPosition::checkIfMouseClickedOnSprite()
{
    int highest = -1;
    int ve = -1;
    std::vector<s2d::Sprite*> spr;

    for (int i = 0; i < s2d::Sprite::activeSprites.size(); i++)
    {
        // Checking if we have the same ID. If we wouldnt do that check it could just return
        // The next sprite in the list which would be wrong
        if (this->m_realeasedCursorOnSprite && sf::Mouse::isButtonPressed(sf::Mouse::Left) && s2d::Sprite::activeSprites[i]->getId() == this->m_clickedSpriteId)
        {
            return s2d::Sprite::activeSprites[i];
        }

        if (checkClick(*s2d::Sprite::activeSprites[i]))
        {
            spr.push_back(s2d::Sprite::activeSprites[i]);
        }
    }

    this->m_windowEvent->type = s2d::Event::None;

    for (s2d::Sprite* sp : spr)
    {
        if (sp->sortingLayerIndex >= highest)
        {
            ve = sp->getVectorPosition();
            highest = sp->sortingLayerIndex;
        }
    }

    if (highest != -1)
    {
        this->m_clickedSprite = s2d::Sprite::activeSprites[ve - 1];
        s2d::UIHirachy::selectedSprite = this->m_clickedSprite;
        return s2d::Sprite::activeSprites[ve - 1];
    }

    return nullptr;
}

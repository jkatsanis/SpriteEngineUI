#include "UIRealTimeEditorTransformPosition.h"

// Constructor


s2d::UIRealTimeEditorTransformPosition::UIRealTimeEditorTransformPosition()
{
    this->m_spriteRepository = nullptr;
    this->m_windowEvent = nullptr;
    this->m_isAnyUIWindowHovered = nullptr;
    this->m_clickedSprite = nullptr;
    this->m_ptr_Inspectorstate = nullptr;
    this->m_realeasedCursorOnSprite = false;
    this->m_clickedSpriteId = -1;
}

s2d::UIRealTimeEditorTransformPosition::UIRealTimeEditorTransformPosition(s2d::InspectorState* ptr_Inspectorstate, bool* isAnyUIWindowHovered, 
    s2d::Event* windowEvent, s2d::SpriteRepository& repo)
{
    this->m_windowEvent = windowEvent;
    this->m_isAnyUIWindowHovered = isAnyUIWindowHovered;
    this->m_ptr_Inspectorstate = ptr_Inspectorstate;
    this->m_clickedSprite = nullptr;
    this->m_realeasedCursorOnSprite = false;
    this->m_clickedSpriteId = -1;

    this->m_spriteRepository = &repo;
}

// Public functions

void s2d::UIRealTimeEditorTransformPosition::update()
{
    if (*this->m_isAnyUIWindowHovered)
    {
        this->m_clickedSprite = nullptr;
        this->m_realeasedCursorOnSprite = false;
        return;
    }
    // Check if we click on a sprite in the editor
    this->m_clickedSprite = this->checkIfMouseClickedOnSprite();

    this->m_cursorWorldPos = s2d::UI::getWorldCordinates();

    if (this->m_clickedSprite != nullptr)
    {
        this->m_clickedSprite = this->m_spriteRepository->sprite_in_inspector;
        this->moveComponent();
    }
    else
    {
        this->m_clickedSprite = nullptr;
        this->m_realeasedCursorOnSprite = false;
    }
}

// Private functions

void s2d::UIRealTimeEditorTransformPosition::moveComponent()
{
    if (this->m_clickedSprite == nullptr) return;
    float x = this->m_cursorWorldPos.x - 960;
    float y = -(this->m_cursorWorldPos.y - 540);

    float m = x - this->m_clickedSprite->transform.position.x;
    float my = y - this->m_clickedSprite->transform.position.y;

    if (s2d::UI::s_game_cursor.posiitonChanged)
    {
        s2d::Vector2 moved = s2d::UI::s_game_cursor.lastPos - s2d::UI::s_game_cursor.position;
        m += moved.x;
        my -= moved.y;
    }

    s2d::Vector2 newPosition = s2d::Vector2(x - m, y - my);

    this->m_clickedSprite->transform.position = newPosition;

}

// Check if clicked on a sprite

bool s2d::UIRealTimeEditorTransformPosition::checkClick(s2d::Sprite* const sprite)
{
    bool collied = s2d::UI::isCursorClickedOnSprite(sprite);


    if (collied && this->m_windowEvent->type == s2d::Event::Type::MousePressedLeft)
    {
        this->m_clickedSpriteId = sprite->getId();

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
    int32_t highest = -1;
    std::string name = "";
    std::vector<s2d::Sprite*> spr;

    for (int i = 0; i < this->m_spriteRepository->amount(); i++)
    {
        s2d::Sprite* const sprite = this->m_spriteRepository->readAt(i);
        // Checking if we have the same ID. If we wouldnt do that check it could just return
        // The next sprite in the list which would be wrong
        if (this->m_realeasedCursorOnSprite && sf::Mouse::isButtonPressed(sf::Mouse::Left) 
            && sprite->getId() == this->m_clickedSpriteId)
        {
            return sprite;
        }

        if (checkClick(sprite))
        {
            spr.push_back(sprite);
        }
    }

    this->m_windowEvent->type = s2d::Event::None;

    for (s2d::Sprite* sp : spr)
    {
        if (sp->sortingLayerIndex >= highest)
        {
            name = sp->name;
            highest = sp->sortingLayerIndex;
        }
    }

    if (highest != -1)
    {
        this->m_clickedSprite = this->m_spriteRepository->getSpriteWithName(name);
        this->m_spriteRepository->sprite_in_inspector = this->m_clickedSprite;
        return this->m_clickedSprite;
    }

    return nullptr;
}


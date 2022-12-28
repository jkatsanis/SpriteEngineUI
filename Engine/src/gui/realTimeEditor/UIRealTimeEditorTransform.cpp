#include "UIRealTimeEditorTransform.h"

// Constructor


s2d::UIRealTimeEditorTransform::UIRealTimeEditorTransform()
{
    this->m_isAnyUIWindowHovered = nullptr;
    this->m_cursorRangeToClick = - 1;
    this->m_ptr_Window = nullptr;   
    this->m_ptr_Inspectorstate = nullptr;
}

s2d::UIRealTimeEditorTransform::UIRealTimeEditorTransform(sf::RenderWindow* window, s2d::InspectorState* ptr_Inspectorstate, bool* isAnyUIWindowHovered)
{
    this->m_isAnyUIWindowHovered = isAnyUIWindowHovered;
    this->m_ptr_Inspectorstate = ptr_Inspectorstate;
    this->m_cursorRangeToClick = 25;
    this->m_ptr_Window = window;
}

// Public functions

void s2d::UIRealTimeEditorTransform::update()
{
    if (*this->m_isAnyUIWindowHovered || s2d::UIHirachy::childSelectedToParent != nullptr || s2d::UIAssetFolder::dragAndDropPath != " ")
    {
        return;
    }
    // Check if we click on a sprite in the editor
    s2d::Sprite* clickedSprite = this->checkIfMouseClickedOnSprite();

    if (clickedSprite != nullptr)
    {
        // Show tools ( transform / scale changer etc ) (UI Shit) 
       
        s2d::UIHirachy::selectedSprite = clickedSprite;
        this->moveComponent();
    }
    if (s2d::UIHirachy::selectedSprite == nullptr)
    {
        *this->m_ptr_Inspectorstate = s2d::InspectorState::GameWindowEditor;
        clickedSprite = nullptr;
    }
}

// Private functions

void s2d::UIRealTimeEditorTransform::moveComponent()
{
    s2d::Sprite* sprite = s2d::UIHirachy::selectedSprite;

    float x = this->m_cursorWorldPos.x;
    float y = this->m_cursorWorldPos.y - 540 ;
    s2d::Vector2 newPosition = s2d::Vector2(x - 960, -y);
    
    sprite->transform.position = newPosition;

}

// Check if clicked on a sprite

bool s2d::UIRealTimeEditorTransform::checkClick(s2d::Sprite& sprite)
{
    sf::Vector2i cursorPos = sf::Mouse::getPosition(*this->m_ptr_Window);
    this->m_cursorWorldPos = this->m_ptr_Window->mapPixelToCoords(cursorPos);

    float getPosX = sprite.getSprite().getPosition().x;
    float getPosY = sprite.getSprite().getPosition().y;

    float otherGetPosX = this->m_cursorWorldPos.x;
    float otherGetPosY = this->m_cursorWorldPos.y;

    return (getPosX + sprite.transform.scale.x >= otherGetPosX
        && getPosX <= otherGetPosX + this->m_cursorRangeToClick
        && getPosY + sprite.transform.scale.y >= otherGetPosY
        && getPosY <= otherGetPosY + this->m_cursorRangeToClick
        && sf::Mouse::isButtonPressed(sf::Mouse::Left));
}

s2d::Sprite* s2d::UIRealTimeEditorTransform::checkIfMouseClickedOnSprite()
{
    for (int i = 0; i < s2d::Sprite::activeSprites.size(); i++)
    {    
        if(checkClick(*s2d::Sprite::activeSprites[i]))
        {
            return s2d::Sprite::activeSprites[i];
        }
    }

    return nullptr;
}

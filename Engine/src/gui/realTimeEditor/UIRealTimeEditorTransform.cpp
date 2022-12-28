#include "UIRealTimeEditorTransform.h"

// Constructor


s2d::UIRealTimeEditorTransform::UIRealTimeEditorTransform()
{
    this->m_cursorRangeToClick = - 1;
    this->m_ptr_Window = nullptr;
}

s2d::UIRealTimeEditorTransform::UIRealTimeEditorTransform(sf::RenderWindow* window, s2d::InspectorState* ptr_Inspectorstate)
{
    this->m_ptr_Inspectorstate = ptr_Inspectorstate;
    this->m_cursorRangeToClick = 25;
    this->m_ptr_Window = window;
}

// Public functions

void s2d::UIRealTimeEditorTransform::update()
{
    // Check if we click on a sprite in the editor
    s2d::Sprite* clickedSprite = this->checkIfMouseClickedOnSprite();

    if (clickedSprite != nullptr)
    {
        // Show tools ( transform / scale changer etc ) (UI Shit) 
        
        std::cout << (int)*this->m_ptr_Inspectorstate << std::endl;
    }
}

// Private functions

bool s2d::UIRealTimeEditorTransform::checkClick(s2d::Sprite& sprite)
{
    sf::Vector2i cursorPos = sf::Mouse::getPosition(*this->m_ptr_Window);
    sf::Vector2f worldPos = this->m_ptr_Window->mapPixelToCoords(cursorPos);

    float getPosX = sprite.getSprite().getPosition().x;
    float getPosY = sprite.getSprite().getPosition().y;

    float otherGetPosX = worldPos.x;
    float otherGetPosY = worldPos.y;

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

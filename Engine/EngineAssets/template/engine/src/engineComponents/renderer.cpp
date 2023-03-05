#include "renderer.h"

// Constructor

s2d::Renderer::Renderer()
{
    this->m_timeToUpdateSpriteTexture = 0.0f;
    this->m_timeToUpdateLayerIndex = 0.0f;
    this->m_timePassedTillNextSpriteTextureUpdate = 0.0f;
    this->m_timePassedToUpdateLayerIndex = 0;
    this->m_ptr_renderWindow = nullptr;
}

s2d::Renderer::Renderer(sf::RenderWindow* renderWindow)
{
    this->m_timePassedToUpdateLayerIndex = 0;
    this->m_ptr_renderWindow = renderWindow;

    this->m_timeToUpdateLayerIndex = 2;
    this->m_timeToUpdateSpriteTexture = 1;

    this->m_timePassedTillNextSpriteTextureUpdate = this->m_timeToUpdateSpriteTexture;
    this->m_timePassedToUpdateLayerIndex = this->m_timeToUpdateLayerIndex;
}

// Private functions

void s2d::Renderer::draw()
{
    this->drawSprites();
    this->drawRectangles();
}


void s2d::Renderer::drawSprites()
{
    this->m_timePassedToUpdateLayerIndex += s2d::Time::deltaTime;
    this->m_timePassedTillNextSpriteTextureUpdate += s2d::Time::deltaTime;

    //2s passed we can update out hightest layer index
    if (this->m_timePassedToUpdateLayerIndex > m_timeToUpdateLayerIndex)
    {
        s2d::Sprite::updateHightestLayerIndex();
        this->m_timePassedToUpdateLayerIndex = 0;
    }

    for (int i = 0; i < s2d::Sprite::highestLayerIndex + 1; i++)
    {
        for (s2d::Sprite* ptr_activeSprites : s2d::Sprite::activeSprites)
        {
            if (ptr_activeSprites->sortingLayerIndex == i)
            {
                ptr_activeSprites->transform.updateTransformPosition();
                this->m_ptr_renderWindow->draw(ptr_activeSprites->getSprite());
            }
        }
    }
}


void s2d::Renderer::drawRectangles()
{
    for (sf::RectangleShape shape : s2d::GameObject::rects)
    {
        this->m_ptr_renderWindow->draw(shape);
    }
}

// Public functions

void s2d::Renderer::render()
{
    sf::Color backgroundColor = sf::Color(sf::Uint8(s2d::FileData::windowBackground.x),
        sf::Uint8(s2d::FileData::windowBackground.y),
        sf::Uint8(s2d::FileData::windowBackground.z));

    this->m_ptr_renderWindow->clear(backgroundColor);
    this->draw();
    ImGui::SFML::Render(*this->m_ptr_renderWindow);
    this->m_ptr_renderWindow->display();

}

// Public static functions

void s2d::Renderer::updateAllSpriteTextures()
{
    for (int i = 0; i < s2d::Sprite::activeSprites.size(); i++)
    {
        s2d::Sprite::activeSprites[i]->setSpriteTexture(s2d::Sprite::activeSprites[i]->getPathOfTextureFile());
    }
}

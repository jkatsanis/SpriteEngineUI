#include "renderer.h"

// Constructor

s2d::Renderer::Renderer()
{
    this->m_timeToUpdateSpriteTexture = 0;
    this->m_timeToUpdateLayerIndex = 0;
    this->m_timePassedToUpdateLayerIndex = 0;
    this->m_ptr_renderWindow = nullptr;
}

s2d::Renderer::Renderer(sf::RenderWindow* renderWindow)
{
    this->m_timePassedToUpdateLayerIndex = 0;
    this->m_ptr_renderWindow = renderWindow;

    this->m_timeToUpdateLayerIndex = 2;
    this->m_timeToUpdateSpriteTexture = 1;

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

    //2s passed we can update out hightest layer index
    if (this->m_timePassedToUpdateLayerIndex > m_timeToUpdateLayerIndex)
    {
        this->m_ptr_repo->updateHighestLayerIndex();
        this->m_timePassedToUpdateLayerIndex = 0;
    }

    for (int i = 0; i < this->m_ptr_repo->getHighestLayerIndex() + 1; i++)
    {
        for (int j = 0; j < this->m_ptr_repo->amount(); j++)
        {
            s2d::Sprite* const sprite = this->m_ptr_repo->readAt(j);
            if (sprite->sortingLayerIndex == i)
            {
                sprite->transform.updateTransformPosition();
                this->m_ptr_renderWindow->draw(sprite->getSprite());
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

void s2d::Renderer::updateAllSpriteTextures(s2d::SpriteRepository& repo)
{
    for (size_t i = 0; i < repo.amount(); i++)
    {
       repo.readAt(i)->setSpriteTexture(repo.readAt(i)->getPathOfTextureFile());
    }
}

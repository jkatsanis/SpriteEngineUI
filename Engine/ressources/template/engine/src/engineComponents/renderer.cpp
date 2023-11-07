#include "renderer.h"

// Constructor

s2d::Renderer::Renderer()
{
    this->m_last_repo_size = 0;
    this->m_ptr_repo = nullptr;
    this->m_timeToUpdateSpriteTexture = 0;
    this->m_timeToUpdateLayerIndex = 0;
    this->m_timePassedToUpdateLayerIndex = 0;
    this->m_ptr_render_window = nullptr;
}

s2d::Renderer::Renderer(sf::RenderWindow* renderWindow)
{
    this->m_ptr_repo = nullptr;
    this->m_last_repo_size = 0;
    this->m_timePassedToUpdateLayerIndex = 0;
    this->m_ptr_render_window = renderWindow;

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
    this->m_timePassedToUpdateLayerIndex += s2d::Time::s_delta_time;

    //2s passed we can update out hightest layer index
    if (this->m_timePassedToUpdateLayerIndex > m_timeToUpdateLayerIndex
        || this->m_last_repo_size != this->m_ptr_repo->amount())
    {
        this->m_ptr_repo->updateHighestLayerIndex();
        this->m_timePassedToUpdateLayerIndex = 0;
    }
    this->m_last_repo_size = this->m_ptr_repo->amount();

    for (size_t i = 0; i < this->m_ptr_repo->getHighestLayerIndex() + 1; i++)
    {
        for (size_t j = 0; j < this->m_ptr_repo->amount(); j++)
        {
            s2d::Sprite* const sprite = this->m_ptr_repo->readAt(j);
            if (sprite->sprite_renderer.sorting_layer_index == i)
            {
                sprite->transform.updateTransformPosition();

                if (sprite->sprite_renderer.effected_by_light)
                {
                    this->m_ptr_render_window->draw(sprite->getSprite(), &s2d::LightRepository::getShader());
                }
                else
                {
                    this->m_ptr_render_window->draw(sprite->getSprite());
                }
            }
        }
    }
}


void s2d::Renderer::drawRectangles()
{
    for (sf::RectangleShape shape : s2d::GameObject::rects)
    {
        this->m_ptr_render_window->draw(shape);
    }
}

// Public functions

void s2d::Renderer::render()
{
    sf::Color backgroundColor = sf::Color(sf::Uint8(this->background_color.x),
        sf::Uint8(this->background_color.y),
        sf::Uint8(this->background_color.z));

    this->m_ptr_render_window->clear(backgroundColor);
    this->draw();
    ImGui::SFML::Render(*this->m_ptr_render_window);
    this->m_ptr_render_window->display();

}

// Public static functions

void s2d::Renderer::updateAllSpriteTextures(s2d::SpriteRepository& repo)
{
    for (size_t i = 0; i < repo.amount(); i++)
    {
       repo.readAt(i)->setSpriteTexture(repo.readAt(i)->getPathOfTextureFile());
    }
}

#include "renderer.h"

// Constructor

s2d::Renderer::Renderer()
{
    this->m_timeToUpdateLayerIndex = 2;
    this->m_timeToUpdateSpriteTexture = 1;

    this->m_timePassedTillNextSpriteTextureUpdate = this->m_timeToUpdateSpriteTexture;
    this->m_timePassedToUpdateLayerIndex = this->m_timeToUpdateLayerIndex;
    this->m_timePassedToUpdateLayerIndex = 0;
    this->m_ptr_background_color = nullptr;
    this->m_ptr_render_window = nullptr;
}

s2d::Renderer::Renderer(sf::RenderWindow* renderWindow, const s2d::Vector3* bg, s2d::SpriteRepository& spritRepo, s2d::GUIRepository& repo)
{
    this->m_ptr_sprite_repo = &spritRepo;
    this->m_timePassedToUpdateLayerIndex = 0;
    this->m_ptr_render_window = renderWindow;
    this->m_ptr_background_color = bg;

    this->m_timeToUpdateLayerIndex = 2;
    this->m_timeToUpdateSpriteTexture = 1;

    this->m_timePassedTillNextSpriteTextureUpdate = this->m_timeToUpdateSpriteTexture;
    this->m_timePassedToUpdateLayerIndex = this->m_timeToUpdateLayerIndex;

    this->m_ptr_gui_repo = &repo;
}

// Private functions

void s2d::Renderer::draw()
{
    this->drawSprites();
    this->drawRectangles();
    this->drawLines();
}


void s2d::Renderer::drawSprites()
{
    this->m_timePassedToUpdateLayerIndex += s2d::Time::deltaTime;
    this->m_timePassedTillNextSpriteTextureUpdate += s2d::Time::deltaTime;

    //2s passed we can update out hightest layer index
    if (this->m_timePassedToUpdateLayerIndex > m_timeToUpdateLayerIndex)
    {
        this->m_ptr_sprite_repo->updateHighestLayerIndex();
        this->m_timePassedToUpdateLayerIndex = 0;
    }
#ifdef LOAD_TEXTURE_FROM_FILES
    if (this->m_timePassedTillNextSpriteTextureUpdate > this->m_timeToUpdateSpriteTexture)
    {
        this->m_timePassedTillNextSpriteTextureUpdate = 0;
        this->updateSriteTextures();
    }
#endif
    for (size_t i = 0; i < this->m_ptr_sprite_repo->getHighestLayerIndex() + 1; i++)
    {
        for (size_t j = 0; j < this->m_ptr_sprite_repo->amount(); j++)
        {
            s2d::Sprite* const sprite = this->m_ptr_sprite_repo->readAt(j);
            if (sprite->sprite_renderer.sorting_layer_index == i)
            {
                sprite->transform.updateTransformPosition();
                this->m_ptr_render_window->draw(sprite->getSprite());
            }
        }
    }
}

void s2d::Renderer::drawLines()
{
    //for (int i = 0; i < s2d::GameObject::lines.size(); i++)
    //{
    //    this->m_ptr_renderWindow->draw(s2d::GameObject::lines[i].line, 2, sf::Lines);
    //}
}

void s2d::Renderer::drawRectangles()
{
    this->m_ptr_gui_repo->render(this->m_ptr_render_window);
}

void s2d::Renderer::updateSriteTextures()
{
    for (int i = 0; i < this->m_ptr_sprite_repo->amount(); i++)
    {
        s2d::Sprite* const sprite = this->m_ptr_sprite_repo->readAt(i);
        sprite->setSpriteTexture(sprite->sprite_renderer.path);
    }
}

// Public functions

void s2d::Renderer::render()
{
    sf::Color backgroundColor = sf::Color(sf::Uint8(this->m_ptr_background_color->x),
                            sf::Uint8(this->m_ptr_background_color->y),
                            sf::Uint8(this->m_ptr_background_color->z));

    this->m_ptr_render_window->clear(backgroundColor);
    this->draw();
    ImGui::SFML::Render(*this->m_ptr_render_window);
    this->m_ptr_render_window->display();

}

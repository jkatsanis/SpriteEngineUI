#include "renderer.h"

// Constructor

s2d::Renderer::Renderer()
{
    this->m_ptr_gui_repo = nullptr;
    this->m_ptr_sprite_repo = nullptr;
    this->m_timeToUpdateLayerIndex = 2;

    this->m_timePassedToUpdateLayerIndex = this->m_timeToUpdateLayerIndex;
    this->m_timePassedToUpdateLayerIndex = 0;
    this->m_ptr_render_window = nullptr;
}

s2d::Renderer::Renderer(sf::RenderWindow* renderWindow, s2d::SpriteRepository& spritRepo, s2d::GUIRepository& repo)
{
    this->m_ptr_sprite_repo = &spritRepo;
    this->m_timePassedToUpdateLayerIndex = 0;
    this->m_ptr_render_window = renderWindow;

    this->m_timeToUpdateLayerIndex = 2;

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
    this->m_timePassedToUpdateLayerIndex += s2d::Time::s_delta_time;

    //2s passed we can update out hightest layer index
    if (this->m_timePassedToUpdateLayerIndex > this->m_timeToUpdateLayerIndex)
    {
        this->m_ptr_sprite_repo->updateHighestLayerIndex();
        this->m_timePassedToUpdateLayerIndex = 0;
    }
    for (size_t i = 0; i < this->m_ptr_sprite_repo->getHighestLayerIndex() + 1; i++)
    {
        for (size_t j = 0; j < this->m_ptr_sprite_repo->amount(); j++)
        {
            s2d::Sprite* const sprite = this->m_ptr_sprite_repo->readAt(j);
            if (sprite->sprite_renderer.sorting_layer_index == i)
            {
                sprite->transform.updateTransformPosition();
                if (sprite->render)
                {
                    if (sprite->effected_by_light)
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

// Public functions

void s2d::Renderer::render()
{
    sf::Color backgroundColor = sf::Color(sf::Uint8(this->m_ptr_gui_repo->background_color.x),
                            sf::Uint8(this->m_ptr_gui_repo->background_color.y),
                            sf::Uint8(this->m_ptr_gui_repo->background_color.z));

    this->m_ptr_render_window->clear(backgroundColor);
    this->draw();
    ImGui::SFML::Render(*this->m_ptr_render_window);
    this->m_ptr_render_window->display();

}

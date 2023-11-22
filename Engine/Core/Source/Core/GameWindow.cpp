#include "GameWindow.h"

spe::GameWindow::GameWindow()
{
	this->m_ptr_RenderGameWindow = nullptr;
}

spe::GameWindow::GameWindow(const spe::Vector2& size, const std::string& name)
{
	this->m_ptr_RenderGameWindow = new sf::RenderWindow(sf::VideoMode((int)size.x, (int)size.y), name, sf::Style::Default);
}

void spe::GameWindow::Draw(spe::Sprite* ptr)
{
	if (ptr->sprite_renderer.effected_by_light)
	{
		this->m_ptr_RenderGameWindow->draw(ptr->getSprite(), &spe::LightRepository::getShader());
	}
	else
	{
		this->m_ptr_RenderGameWindow->draw(ptr->getSprite());
	}
}

void spe::GameWindow::Display()
{
	this->m_ptr_RenderGameWindow->display();
}

void spe::GameWindow::Clear()
{
	sf::Color backgroundColor = sf::Color(sf::Uint8(this->BackgroundColor.x),
		sf::Uint8(this->BackgroundColor.y),
		sf::Uint8(this->BackgroundColor.z));

	this->m_ptr_RenderGameWindow->clear(backgroundColor);
}


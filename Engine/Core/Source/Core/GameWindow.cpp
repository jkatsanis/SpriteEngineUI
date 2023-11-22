#include "GameWindow.h"

spe::GameWindow::GameWindow()
{
	this->m_ptr_RenderGameWindow = nullptr;
}

spe::GameWindow::GameWindow(const spe::Vector2& size, const std::string& name)
{
	this->m_ptr_RenderGameWindow = new sf::RenderWindow(sf::VideoMode((int)size.x, (int)size.y), name, sf::Style::Default);
}

void spe::GameWindow::PollEvents()
{
	bool eventChanged = false;

	while (this->m_ptr_RenderGameWindow->pollEvent(this->WindowEvent))
	{
		ImGui::SFML::ProcessEvent(this->WindowEvent);

		if (this->WindowEvent.type == sf::Event::Closed)
		{
			this->m_ptr_RenderGameWindow->close();
		}
		if (!eventChanged)
		{
			if (this->WindowEvent.type == sf::Event::KeyReleased)
			{
				Event.type = spe::Event::Released;
				eventChanged = true;
			}
			else if (this->WindowEvent.type == sf::Event::KeyPressed)
			{
				eventChanged = true;
				Event.type = spe::Event::Pressed;
			}
			else
			{
				eventChanged = true;
				Event.type = spe::Event::None;
			}
			Event.key = static_cast<spe::KeyBoardCode>(static_cast<sf::Keyboard::Key>(this->WindowEvent.key.code));
		}
	}
	ImGui::SFML::Update(*this->m_ptr_RenderGameWindow, spe::Time::s_delta_clock.restart());
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


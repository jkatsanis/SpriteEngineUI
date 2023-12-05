#include "GameWindow.h"

// Ctor

spe::GameWindow::GameWindow()
{
	this->m_IsOpen = false;
	this->m_BackgroundColor = nullptr;
	this->m_Camera = nullptr;
	this->WindowEvent.type = sf::Event::GainedFocus;
	this->m_ptr_Window = nullptr;
}

spe::GameWindow::GameWindow(const spe::Vector2& size, const std::string& name)
{
	this->m_BackgroundColor = nullptr;
	this->m_Size = size;
	this->m_Camera = nullptr;
	this->WindowEvent.type = sf::Event::GainedFocus;
	this->m_ptr_Window = new sf::RenderWindow(sf::VideoMode((int)size.X, (int)size.Y), name, sf::Style::Default);
	this->m_IsOpen = true;

	this->m_WindowBounds = sf::IntRect(0, 0, this->m_ptr_Window->getSize().x, this->m_ptr_Window->getSize().y);

	ImGui::SFML::Init(*this->m_ptr_Window);
}

// Private

void spe::GameWindow::UpdateCamera()
{
	if (this->m_Camera != nullptr)
	{
		this->m_Camera->CameraView.setSize(this->m_Size.X * this->m_Camera->GetZoom(), this->m_Size.Y * this->m_Camera->GetZoom());
		this->m_ptr_Window->setView(this->m_Camera->CameraView);
	}
}

// Public

void spe::GameWindow::PollEvents()
{
	bool EventChanged = false;
	Event.Type = spe::Event::None;

	while (this->m_ptr_Window->pollEvent(this->WindowEvent))
	{
		ImGui::SFML::ProcessEvent(this->WindowEvent);

		if (this->WindowEvent.type == sf::Event::Closed)
		{
			this->m_IsOpen = false;
			this->m_ptr_Window->close();
		}
		if (!EventChanged)
		{
			if (this->WindowEvent.type == sf::Event::KeyReleased)
			{
				Event.Type = spe::Event::KeyReleased;
				EventChanged = true;
			}
			else if (this->WindowEvent.type == sf::Event::KeyPressed)
			{
				EventChanged = true;
				Event.Type = spe::Event::KeyPressed;
			}
			else if (this->WindowEvent.type == sf::Event::MouseButtonPressed)
			{
				if (this->WindowEvent.mouseButton.button == sf::Mouse::Left)
				{
					Event.Type = spe::Event::MousePressedLeft;
					EventChanged = true;
				}
			}
			else if (this->WindowEvent.type == sf::Event::MouseButtonReleased)
			{
				if (this->WindowEvent.mouseButton.button == sf::Mouse::Left)
				{
					Event.Type = spe::Event::MouseReleasedLeft;
					EventChanged = true;
				}
			}
			else
			{
				EventChanged = true;
				Event.Type = spe::Event::None;
			}
			Event.Key = static_cast<spe::KeyBoardCode>(static_cast<sf::Keyboard::Key>(this->WindowEvent.key.code));
		}
	}
	ImGui::SFML::Update(*m_ptr_Window, Time::s_DeltaClock.restart());
}

void spe::GameWindow::Draw(spe::Sprite* ptr, const sf::Shader* shader)
{
	if (!ptr->SpriteRenderer.render)
	{
		return;
	}
	if (shader != nullptr && ptr->SpriteRenderer.effected_by_light)
	{
		this->m_ptr_Window->draw(ptr->GetSprite(), shader);
	}
	else 
	{
		this->m_ptr_Window->draw(ptr->GetSprite());
	}
}

void spe::GameWindow::Display()
{
	ImGui::SFML::Render(*this->m_ptr_Window);

	this->UpdateCamera();
	this->m_ptr_Window->display();
}

void spe::GameWindow::Clear()
{
	sf::Color backgroundColor = sf::Color(0, 0, 0);

	if (this->m_BackgroundColor != nullptr)
	{
		backgroundColor = sf::Color(sf::Uint8(this->m_BackgroundColor->X),
			sf::Uint8(this->m_BackgroundColor->Y),
			sf::Uint8(this->m_BackgroundColor->Z));
	}
	this->m_ptr_Window->clear(backgroundColor);
}

void spe::GameWindow::Shutdown()
{
	ImGui::SFML::Shutdown();
	delete this->m_ptr_Window;
	this->m_IsOpen = false;
}

bool spe::GameWindow::ContainsCursor()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->m_ptr_Window);
	return this->m_WindowBounds.contains(mousePosition);
}
	


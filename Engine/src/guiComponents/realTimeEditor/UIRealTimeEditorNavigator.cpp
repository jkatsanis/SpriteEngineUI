#include "UIRealTimeEditorNavigator.h"


//Constructor

s2d::UIRealTimeEditorNavigator::UIRealTimeEditorNavigator()
{
	this->m_ptr_gui_repo = nullptr;
	this->m_isAnyUIWindowHovered = nullptr;
	this->m_ptr_event_engine = nullptr;
	this->m_ptr_renderWindow = nullptr;
	this->m_arrowSpeed = 0.0f;
	this->m_scrollSpeed = 0.0f;
}

s2d::UIRealTimeEditorNavigator::UIRealTimeEditorNavigator(sf::RenderWindow& window, sf::Event* event, bool* isAnyUIWindowHovered, s2d::GUIRepository& repo)
{
	this->m_ptr_gui_repo = &repo;
	this->m_ptr_renderWindow = &window;
	this->m_isAnyUIWindowHovered = isAnyUIWindowHovered;
	this->m_ptr_event_engine = event;
	this->m_arrowSpeed = 400;
	this->m_scrollSpeed = 0.15f;
	
	const std::string path_to_texture = PATH_TO_RESSOURCS"/Sprites/transparent.png";

	this->m_ptr_gui_repo->add(sf::Vector2f(0, 0), sf::Vector2f(1920, 1080),
		sf::Color(255, 255, 255), 4.0f, path_to_texture, "white-box");
}

//Public functions

void s2d::UIRealTimeEditorNavigator::update()
{
	this->setChangedPosition();

	//Camera update (updating every frame)
	this->m_ptr_gui_repo->camera.update();

	if (*this->m_isAnyUIWindowHovered) return;

	this->navigateRightClick();
	this->navigateArrows();
	this->navigateScrollWheel();

}

//Private functions

void s2d::UIRealTimeEditorNavigator::navigateRightClick()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(*this->m_ptr_renderWindow);

		if (this->m_cursor.position_changed)
		{
			s2d::Vector2 moved = this->m_cursor.lastPos - this->m_cursor.position;

			this->m_ptr_gui_repo->camera.transform.position += moved;
		}
	}
}

void s2d::UIRealTimeEditorNavigator::navigateScrollWheel()
{
	if (this->m_ptr_event_engine->type == sf::Event::MouseWheelScrolled)
	{
		this->m_ptr_event_engine->type = sf::Event::GainedFocus;

		if (this->m_ptr_event_engine->mouseWheel.x < 0)
		{
			if (this->m_ptr_gui_repo->camera.cameraZoom + this->m_scrollSpeed < 4)
			{
				this->m_ptr_gui_repo->camera.cameraZoom += this->m_scrollSpeed;
			}
		}
		else
		{
			if (this->m_ptr_gui_repo->camera.cameraZoom - this->m_scrollSpeed > 0.04)
			{
				this->m_ptr_gui_repo->camera.cameraZoom -= this->m_scrollSpeed;
			}
		}
	}
}

void s2d::UIRealTimeEditorNavigator::calculateScrollWheelSpeed()
{
	sf::Vector2f size = this->m_ptr_gui_repo->camera.cameraView.getSize();

	if (size.x > 1500)
	{
		this->m_scrollSpeed = 0.006f;
	}
	else if (size.x > 1000)
	{
		this->m_scrollSpeed = 0.005f;
	}
	else if (size.x > 750)
	{
		this->m_scrollSpeed = 0.004f;
	}
	else
	{
		this->m_scrollSpeed = 0.0008f;
	}
}

void s2d::UIRealTimeEditorNavigator::setChangedPosition()
{
	this->m_cursor.position = s2d::Vector2(float(sf::Mouse::getPosition(*this->m_ptr_renderWindow).x), float(sf::Mouse::getPosition(*this->m_ptr_renderWindow).y));

	this->m_cursor.setLastPosition();
}

void s2d::UIRealTimeEditorNavigator::navigateArrows()
{
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::Right))
	{
		this->m_ptr_gui_repo->camera.transform.position.x += this->m_arrowSpeed * s2d::Time::s_delta_time;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::Left))
	{
		this->m_ptr_gui_repo->camera.transform.position.x -= this->m_arrowSpeed * s2d::Time::s_delta_time;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::Up))
	{
		this->m_ptr_gui_repo->camera.transform.position.y -= this->m_arrowSpeed * s2d::Time::s_delta_time;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::Down))
	{
		this->m_ptr_gui_repo->camera.transform.position.y += this->m_arrowSpeed * s2d::Time::s_delta_time;
	}
}
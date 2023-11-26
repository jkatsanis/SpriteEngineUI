#include "UIRealTimeEditorNavigator.h"


//Constructor


void spe::UIRealTimeEditorNavigator::Init()
{
	// White box :D
	spe::Rectangle* rec = new spe::Rectangle(sf::Vector2f(0, 0), sf::Vector2f(1920, 1080), sf::Color(255, 255, 255), 3.5f,
		PATH_TO_TRANSPARENT_PIC, "game-rec");

	this->m_ptr_GUIRepo->Add(rec);
}

//Public functions

void spe::UIRealTimeEditorNavigator::Render()
{
	this->setChangedPosition();

	//Camera update (updating every frame)
	this->m_ptr_GUIRepo->Camera.Update();

	if (spe::UIUtility::s_IsAnyHovered) return;

	this->navigateRightClick();
	this->navigateArrows();
	this->navigateScrollWheel();
	this->navigateKeys();
	this->calculateScrollWheelSpeed();

}

//Private functions

void spe::UIRealTimeEditorNavigator::navigateRightClick()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(*this->m_ptr_Window->GetRenderWindow());

		if (this->m_cursor.PositionChanged)
		{
			spe::Vector2 moved = this->m_cursor.LastPosition - this->m_cursor.Position;

			moved = moved * this->m_ptr_GUIRepo->Camera.getZoom();

			this->m_ptr_GUIRepo->Camera.Position += moved;
		}
	}
}

void spe::UIRealTimeEditorNavigator::navigateScrollWheel()
{
	if (this->m_ptr_Window->WindowEvent.type == sf::Event::MouseWheelScrolled)
	{
		this->m_ptr_Window->WindowEvent.type = sf::Event::GainedFocus;
		const float zoom = this->m_ptr_GUIRepo->Camera.getZoom();
		if (this->m_ptr_Window->WindowEvent.mouseWheel.x < 0)
		{
			if (zoom + this->m_scroll_speed < 4)
			{
				this->m_ptr_GUIRepo->Camera.setZoom(zoom + this->m_scroll_speed * 30);
			}
		}
		else
		{
			if (zoom - this->m_scroll_speed > 0.04)
			{
				this->m_ptr_GUIRepo->Camera.setZoom(zoom - this->m_scroll_speed * 30);
			}
		}
	}
}

void spe::UIRealTimeEditorNavigator::calculateScrollWheelSpeed()
{
	sf::Vector2f size = this->m_ptr_GUIRepo->Camera.camera_view.getSize();

	if (size.x > 1500)
	{
		this->m_scroll_speed = 0.006f;
	}
	else if (size.x > 1000)
	{
		this->m_scroll_speed = 0.005f;
	}
	else if (size.x > 750)
	{
		this->m_scroll_speed = 0.004f;
	}
	else
	{
		this->m_scroll_speed = 0.0008f;
	}
}

void spe::UIRealTimeEditorNavigator::setChangedPosition()
{
	this->m_cursor.Position = spe::Vector2(float(sf::Mouse::getPosition(*this->m_ptr_Window->GetRenderWindow()).x), 
		float(sf::Mouse::getPosition(*this->m_ptr_Window->GetRenderWindow()).y));

	this->m_cursor.SetLastPosition();
}

void spe::UIRealTimeEditorNavigator::navigateArrows()
{
	const float camera_speed = this->m_ptr_GUIRepo->Camera.camera_speed;
	if (spe::Input::onKeyHold(spe::KeyBoardCode::Right))
	{
		this->m_ptr_GUIRepo->Camera.Position.x += camera_speed * spe::Time::s_delta_time;
	}
	if (spe::Input::onKeyHold(spe::KeyBoardCode::Left))
	{
		this->m_ptr_GUIRepo->Camera.Position.x -= camera_speed * spe::Time::s_delta_time;
	}
	if (spe::Input::onKeyHold(spe::KeyBoardCode::Up))
	{
		this->m_ptr_GUIRepo->Camera.Position.y -= camera_speed * spe::Time::s_delta_time;
	}
	if (spe::Input::onKeyHold(spe::KeyBoardCode::Down))
	{
		this->m_ptr_GUIRepo->Camera.Position.y += camera_speed * spe::Time::s_delta_time;
	}
}

void spe::UIRealTimeEditorNavigator::navigateKeys()
{
	const float camera_speed = this->m_ptr_GUIRepo->Camera.camera_speed;

	if (spe::Input::onKeyHold(spe::KeyBoardCode::D))
	{
		this->m_ptr_GUIRepo->Camera.Position.x += camera_speed * spe::Time::s_delta_time;
	}
	if (spe::Input::onKeyHold(spe::KeyBoardCode::A))
	{
		this->m_ptr_GUIRepo->Camera.Position.x -= camera_speed * spe::Time::s_delta_time;
	}
	if (spe::Input::onKeyHold(spe::KeyBoardCode::W))
	{
		this->m_ptr_GUIRepo->Camera.Position.y -= camera_speed * spe::Time::s_delta_time;
	}
	if (spe::Input::onKeyHold(spe::KeyBoardCode::S) && !spe::Input::onKeyHold(spe::KeyBoardCode::LControl))
	{
		this->m_ptr_GUIRepo->Camera.Position.y += camera_speed * spe::Time::s_delta_time;
	}
}

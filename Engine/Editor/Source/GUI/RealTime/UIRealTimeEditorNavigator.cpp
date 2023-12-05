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
	this->SetChangedPosition();

	if (spe::UIUtility::s_IsAnyHovered) return;

	this->NavigateRightClick();
	this->NavigateArrows();
	this->NavigateScrollWheel();
	this->NavigateKeys();
	this->CalculateScrollWheelSpeed();

}

//Private functions

void spe::UIRealTimeEditorNavigator::NavigateRightClick()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(*this->m_ptr_Window->GetRenderWindow());

		if (this->m_Cursor.PositionChanged)
		{
			spe::Vector2 moved = this->m_Cursor.LastPosition - this->m_Cursor.Position;

			moved = moved * this->m_ptr_GUIRepo->Camera.GetZoom();

			this->m_ptr_GUIRepo->Camera.Position += moved;
		}
	}
}

void spe::UIRealTimeEditorNavigator::NavigateScrollWheel()
{
	if (this->m_ptr_Window->WindowEvent.type == sf::Event::MouseWheelScrolled)
	{
		this->m_ptr_Window->WindowEvent.type = sf::Event::GainedFocus;
		const float zoom = this->m_ptr_GUIRepo->Camera.GetZoom();
		if (this->m_ptr_Window->WindowEvent.mouseWheel.x < 0)
		{
			if (zoom + this->m_ScrollSpeed < 4)
			{
				this->m_ptr_GUIRepo->Camera.SetZoom(zoom + this->m_ScrollSpeed * 30);
			}
		}
		else
		{
			if (zoom - this->m_ScrollSpeed > 0.04)
			{
				this->m_ptr_GUIRepo->Camera.SetZoom(zoom - this->m_ScrollSpeed * 30);
			}
		}
	}
}

void spe::UIRealTimeEditorNavigator::CalculateScrollWheelSpeed()
{
	sf::Vector2f size = this->m_ptr_GUIRepo->Camera.CameraView.getSize();

	if (size.x > 1500)
	{
		this->m_ScrollSpeed = 0.006f;
	}
	else if (size.x > 1000)
	{
		this->m_ScrollSpeed = 0.005f;
	}
	else if (size.x > 750)
	{
		this->m_ScrollSpeed = 0.004f;
	}
	else
	{
		this->m_ScrollSpeed = 0.0008f;
	}
}

void spe::UIRealTimeEditorNavigator::SetChangedPosition()
{
	this->m_Cursor.Position = spe::Vector2(float(sf::Mouse::getPosition(*this->m_ptr_Window->GetRenderWindow()).x), 
		float(sf::Mouse::getPosition(*this->m_ptr_Window->GetRenderWindow()).y));

	this->m_Cursor.SetLastPosition();
}

void spe::UIRealTimeEditorNavigator::NavigateArrows()
{
	const float camera_speed = this->m_ptr_GUIRepo->Camera.CameraSpeed;
	if (spe::Input::OnKeyHold(spe::KeyBoardCode::Right))
	{
		this->m_ptr_GUIRepo->Camera.Position.X += camera_speed * spe::Time::s_DeltaTime;
	}
	if (spe::Input::OnKeyHold(spe::KeyBoardCode::Left))
	{
		this->m_ptr_GUIRepo->Camera.Position.X -= camera_speed * spe::Time::s_DeltaTime;
	}
	if (spe::Input::OnKeyHold(spe::KeyBoardCode::Up))
	{
		this->m_ptr_GUIRepo->Camera.Position.Y -= camera_speed * spe::Time::s_DeltaTime;
	}
	if (spe::Input::OnKeyHold(spe::KeyBoardCode::Down))
	{
		this->m_ptr_GUIRepo->Camera.Position.Y += camera_speed * spe::Time::s_DeltaTime;
	}
}

void spe::UIRealTimeEditorNavigator::NavigateKeys()
{
	const float camera_speed = this->m_ptr_GUIRepo->Camera.CameraSpeed;

	if (spe::Input::OnKeyHold(spe::KeyBoardCode::D))
	{
		this->m_ptr_GUIRepo->Camera.Position.X += camera_speed * spe::Time::s_DeltaTime;
	}
	if (spe::Input::OnKeyHold(spe::KeyBoardCode::A))
	{
		this->m_ptr_GUIRepo->Camera.Position.X -= camera_speed * spe::Time::s_DeltaTime;
	}
	if (spe::Input::OnKeyHold(spe::KeyBoardCode::W))
	{
		this->m_ptr_GUIRepo->Camera.Position.Y -= camera_speed * spe::Time::s_DeltaTime;
	}
	if (spe::Input::OnKeyHold(spe::KeyBoardCode::S) && !spe::Input::OnKeyHold(spe::KeyBoardCode::LControl))
	{
		this->m_ptr_GUIRepo->Camera.Position.Y += camera_speed * spe::Time::s_DeltaTime;
	}
}
